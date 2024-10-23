#include "DxLib.h"
#include "DebugDraw.h"
#include "LoadCsv.h"
#include "Stage.h"
#include "Player.h"
#include "Weapon.h"

// 定数
namespace
{
	const std::string kWeaponFileName = "data/model/weapon/";		// モデルのファイルパス名
	const char* kPlayerHandFrameName = "mixamorig:LeftHandIndex2";  // プレイヤーの手の部分のフレーム名
}

Weapon::Weapon() :
	m_locationDataHandle(-1)
{
	LoadLocationData(); // 配置データの読み込み
}

Weapon::~Weapon()
{
	for (auto& pair : m_objHandle)
	{
		MV1DeleteModel(pair.second);
	}
}

void Weapon::Init()
{
	// サイズや位置の調整
	for (auto& loc : m_locationData)
	{
		LoadCsv::GetInstance().LoadWeaponData(m_weaponData, loc.name);
		m_durability = m_weaponData.durability;
		m_updateCol.colRadius = m_weaponData.colRadius;
		MV1SetScale(m_objHandle[loc.name], loc.scale);
		MV1SetPosition(m_objHandle[loc.name], loc.pos);
	}
}

void Weapon::Update(Player& player, Stage& stage)
{
	MATRIX frameMatrix = {}; // 武器の回転行列

	// 武器位置更新
	for (auto& loc : m_locationData)
	{
		// プレイヤーが武器を掴んだ場合、プレイヤーの手の位置に武器を移動させる
		if (player.GetIsGrabWeapon())
		{
			SetModelFramePos(player.GetHandle(), kPlayerHandFrameName, m_objHandle[loc.name], loc);
		}
		else
		{
			loc.pos = VAdd(loc.pos, VGet(0.0f, m_gravity, 0.0f)); // 重力を足す
			loc.pos = VAdd(VGet(loc.pos.x, 0.0f, loc.pos.z), stage.CheckObjectCol(*this, VGet(0.0f, 0.0f, 0.0f))); // ステージと当たり判定を行う
		}

		// TODO:バトル終了後、武器位置をリセットする
		// 今は仮で所持金300になったら
		if (player.GetMoney() == 300)
		{
			// 武器の位置を初期位置にリセット
			loc.pos = loc.initPos;
			loc.rot = loc.initRot;
		}
		
		UpdateCol(loc); // 当たり判定位置更新
		MV1SetPosition(m_objHandle[loc.name], loc.pos);
		MV1SetRotationXYZ(m_objHandle[loc.name], loc.rot);
	}
}

void Weapon::Draw()
{
	for (auto& loc : m_locationData)
	{
		MV1DrawModel(m_objHandle[loc.name]); // モデル表示
	}

#ifdef _DEBUG
	DebugDraw debug;
	for (const auto& loc : m_locationData)
	{
		// 武器情報描画
		debug.DrawWeaponInfo(loc.name.c_str(), loc.tag.c_str(), loc.pos, loc.rot, loc.scale, m_durability);
		// 当たり判定描画
		debug.DrawWeaponCol(m_updateCol.colStartPos, m_updateCol.colEndPos, m_updateCol.colRadius);
	}
#endif
}

void Weapon::LoadLocationData()
{
	m_locationDataHandle = FileRead_open((kWeaponFileName + "locationData.loc").c_str());

	int dataCnt = 0; // データ数
	FileRead_read(&dataCnt, sizeof(dataCnt), m_locationDataHandle);
	m_locationData.resize(dataCnt);

	for (auto& loc : m_locationData)
	{
		// オブジェクト名をロード
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), m_locationDataHandle);
		loc.name.resize(nameCnt);

		// MEMO:loc.name.data()の部分はC++20だとエラーにならない
		FileRead_read(loc.name.data(), sizeof(char)* loc.name.size(), m_locationDataHandle);

		// タグをロード
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), m_locationDataHandle);
		loc.tag.resize(tagCnt);
		FileRead_read(loc.tag.data(), sizeof(char) * loc.tag.size(), m_locationDataHandle);

		// 座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), m_locationDataHandle);
		loc.initPos = loc.pos;
		// 回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), m_locationDataHandle);
		loc.initRot = loc.rot;
		// スケール情報
		FileRead_read(&loc.scale, sizeof(loc.scale), m_locationDataHandle);
	}
	FileRead_close(m_locationDataHandle);

	// モデルのパスを設定
	for (auto& loc : m_locationData)
	{
		if (m_objHandle.find(loc.name) == m_objHandle.end())
		{
			std::string modelPath = kWeaponFileName + loc.name + ".mv1";
			int modelHandle = MV1LoadModel(modelPath.c_str());
			m_objHandle[loc.name] = modelHandle;
		}
	}
}

void Weapon::UpdateCol(auto& loc)
{
	// 向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(loc.rot.y);

	// 当たり判定位置を更新
	m_updateCol.colStartPos = VAdd(loc.pos, (VTransform(m_weaponData.colStartPos, rotationMatrix)));
	m_updateCol.colEndPos = VAdd(m_updateCol.colStartPos, (VTransform(m_weaponData.colEndPos, rotationMatrix)));
}

void Weapon::CheckWeaopnCol(const CharacterBase::ColData& colData, Player& player)
{
	bool isHit = HitCheck_Capsule_Capsule(m_updateCol.colStartPos, m_updateCol.colEndPos, m_updateCol.colRadius, colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);

	// 当たっている場合
	if (isHit)
	{
		// 武器を掴めるようにする
		player.SetIsPossibleGrabWeapon(true);
	}
	else
	{
		player.SetIsPossibleGrabWeapon(false);
	}
}

void Weapon::SetModelFramePos(int modelHandle, const char* frameName, int setModelHandle, auto& loc)
{
	// フレーム名からフレーム番号を取得する
	int frameIndex = MV1SearchFrame(modelHandle, frameName);
	MATRIX frameMatrix = MV1GetFrameLocalWorldMatrix(modelHandle, frameIndex);

	// 武器位置を更新
	loc.pos = VTransform(VGet(0.0f, 0.0f, 0.0f), frameMatrix);
	loc.rot = VTransform(VGet(0.0f, 0.0f, 0.0f), frameMatrix);
	MV1SetMatrix(setModelHandle, frameMatrix);
}