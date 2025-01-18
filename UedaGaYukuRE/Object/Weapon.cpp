#include "DxLib.h"
#include "DebugDraw.h"
#include "LoadCsv.h"
#include "Stage.h"
#include "Player.h"
#include "Weapon.h"

// 定数
namespace
{
	const char* kTextHandleFileName = "data/ui/text/hirou.png";		// 画像ハンドルのパス名
	const std::string kWeaponFileName = "data/model/weapon/";		// モデルのファイルパス名
	constexpr int kPlayerHandFrameNum = 51;	// 武器をアタッチするフレーム番号
	constexpr float kGroundHeight = 41.0f;  // 地面の高さ
	const float kDispTextAdjY = 25.0f; // 拾うのテキスト調整位置
}

Weapon::Weapon(std::shared_ptr<Player> pPlayer) :
	m_loadLocationData(-1),
	m_isHitAttack(false)
{
	m_pPlayer = pPlayer;
	m_handle = LoadGraph(kTextHandleFileName);
	LoadLocationData(); // 配置データの読み込み
}

Weapon::~Weapon()
{
	DeleteGraph(m_handle);
	for (auto& pair : m_objHandle)
	{
		MV1DeleteModel(pair.second);
	}
}

void Weapon::Init()
{
	LoadCsv::GetInstance().LoadWeaponData(m_weaponData);

	// サイズや位置の調整
	for (auto& loc : m_locationData)
	{
		loc.durability = m_weaponData[loc.name].durability;
		loc.updateCol.colRadius = m_weaponData[loc.name].colRadius;
		MV1SetPosition(m_objHandle[loc.name], loc.pos);
		MV1SetScale(m_objHandle[loc.name], loc.scale);
	}
}

void Weapon::Update(Stage& stage)
{
	MATRIX frameMatrix = {}; // 武器の回転行列

	// 武器位置更新
	for (auto& loc : m_locationData)
	{
		// バトル中でない場合
		if (!m_pPlayer->GetIsBattle())
		{
			// 武器の位置を初期位置にリセット
			loc.pos = loc.initPos;
			loc.rot = loc.initRot;
			loc.durability = m_weaponData[loc.name].durability;
			loc.isGrab = false;
			loc.rot = VGet(0.0f, 0.0f, 0.0f); // 回転を初期化
			frameMatrix = MGetIdent();		  // 単位行列を設定
			MV1SetMatrix(m_objHandle[loc.name], frameMatrix);
		}
		// バトル中の場合
		else
		{
			// 耐久力が0になった場合
			if (loc.durability <= 0)
			{
				// モデルを非表示し、当たり判定を消す
				MV1SetFrameVisible(m_objHandle[loc.name], 0, false);
				m_pPlayer->SetIsGrabWeapon(false); // プレイヤーの武器掴み状態を解除する
				loc.isGrab = false;
				loc.durability = std::max(loc.durability, 0);
			}
			else
			{
				MV1SetFrameVisible(m_objHandle[loc.name], 0, true);
			}

			// プレイヤーが武器を掴んだ場合、プレイヤーの手の位置に武器を移動させる
			if (loc.isGrab)
			{
				SetModelFramePos(loc, frameMatrix);
			}
			else
			{
				loc.rot = VGet(0.0f, 0.0f, 0.0f); // 回転を初期化
				frameMatrix = MGetIdent();		  // 単位行列を設定
				MV1SetMatrix(m_objHandle[loc.name], frameMatrix);
			}
		}

		loc.pos = VAdd(loc.pos, VGet(0.0f, m_gravity, 0.0f)); // 重力を足す
		loc.pos = VAdd(VGet(loc.pos.x, 0.0f, loc.pos.z), stage.CheckObjectCol(*this, VGet(0.0f, 0.0f, 0.0f))); // ステージと当たり判定を行う
		loc.pos.y = std::max(kGroundHeight, loc.pos.y);
		
		UpdateCol(loc); // 当たり判定位置更新
		MV1SetPosition(m_objHandle[loc.name], loc.pos);
		MV1SetRotationXYZ(m_objHandle[loc.name], loc.rot);
		MV1SetScale(m_objHandle[loc.name], loc.scale);
	}
}

void Weapon::Draw()
{
	for (auto& loc : m_locationData)
	{
		MV1DrawModel(m_objHandle[loc.name]); // モデル表示
	}

	// バトル中でない場合は表示しない
	if(!m_pPlayer->GetIsBattle()) return;

#ifdef _DEBUG
	DebugDraw debug;
	int dispY = 440; // 武器情報デバッグ表示位置
	for (const auto& loc : m_locationData)
	{
		// 武器情報描画
		//debug.DrawWeaponInfo(loc.name.c_str(), loc.tag.c_str(), loc.pos, loc.rot, loc.scale, loc.durability, dispY);
		//dispY += 20;

		if (loc.durability <= 0) continue;
		
		// 当たり判定描画
		debug.DrawWeaponCol(loc.updateCol.colStartPos, loc.updateCol.colEndPos, loc.updateCol.colRadius);
	
	}
#endif
}

void Weapon::DrawWeaponUi()
{
	// バトル中でない場合は表示しない
	if (!m_pPlayer->GetIsBattle()) return;

	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->GetIsGrabWeapon()) return;

		if (loc.durability <= 0) continue;

		// プレイヤーが武器に近づいたら拾うUIを表示する
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			// UIの位置を計算する
			VECTOR modelTopPos = VAdd(loc.pos, VGet(0.0f, kDispTextAdjY, 0.0f));
			VECTOR screenPos = ConvWorldPosToScreenPos(modelTopPos);

			DrawGraphF(screenPos.x, screenPos.y, m_handle, true);
		}
	}
}

void Weapon::DecrementDurability()
{
	if (m_isHitAttack) return;
	
	for (auto& loc : m_locationData)
	{
		if (loc.isGrab)
		{
			loc.durability--;  // 耐久力を1減らす
			m_isHitAttack = true;
		}
	}
}

void Weapon::UpdateIsGrab(bool isGrab)
{
	for (auto& loc : m_locationData)
	{
		if (isGrab)
		{
			if (m_pPlayer->IsNearWeapon(loc.pos))
			{
				loc.isGrab = true;
			}
		}
		else
		{
			loc.isGrab = false;
		}
	}
}

std::string Weapon::GetNearWeaponTag() const
{
	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			return loc.tag;
		}
	}

	return "";
}

void Weapon::LoadLocationData()
{
	m_loadLocationData = FileRead_open((kWeaponFileName + "locationData.loc").c_str());

	int dataCnt = 0; // データ数
	FileRead_read(&dataCnt, sizeof(dataCnt), m_loadLocationData);
	m_locationData.resize(dataCnt);

	for (auto& loc : m_locationData)
	{
		// オブジェクト名をロード
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), m_loadLocationData);
		loc.name.resize(nameCnt);

		// MEMO:loc.name.data()の部分はC++20だとエラーにならない
		FileRead_read(loc.name.data(), static_cast<int>(sizeof(char)* loc.name.size()), m_loadLocationData);

		// タグをロード
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), m_loadLocationData);
		loc.tag.resize(tagCnt);
		FileRead_read(loc.tag.data(), static_cast<int>(sizeof(char) * loc.tag.size()), m_loadLocationData);

		// 座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), m_loadLocationData);
		loc.initPos = loc.pos;
		// 回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), m_loadLocationData);
		loc.initRot = loc.rot;
		// スケール情報
		FileRead_read(&loc.scale, sizeof(loc.scale), m_loadLocationData);
	}
	FileRead_close(m_loadLocationData);

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
	//MATRIX rotationMatrix = MMult(MMult(MGetRotX(loc.rot.x), MGetRotY(loc.rot.y)), MGetRotZ(loc.rot.z));
	MATRIX rotationMatrix = MGetRotY(loc.rot.z);

	// 当たり判定位置を更新
	loc.updateCol.colStartPos = VAdd(loc.pos, (VTransform(m_weaponData[loc.name].colStartPos, rotationMatrix)));
	loc.updateCol.colEndPos = VAdd(loc.updateCol.colStartPos, (VTransform(m_weaponData[loc.name].colEndPos, rotationMatrix)));
}

bool Weapon::CheckWeaponCol(const CharacterBase::ColData& colData, Player& player)
{
	for (auto& loc : m_locationData)
	{
		if (m_pPlayer->IsNearWeapon(loc.pos))
		{
			return HitCheck_Capsule_Capsule(loc.updateCol.colStartPos, loc.updateCol.colEndPos, loc.updateCol.colRadius, colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius);
		}	
	}

	return false;
}

void Weapon::SetModelFramePos(auto& loc, MATRIX frameMatrix)
{
	frameMatrix = MV1GetFrameLocalWorldMatrix(m_pPlayer->GetHandle(), kPlayerHandFrameNum);

	// 武器位置を更新
	loc.pos = VTransform(VGet(0.0f, 0.0f, 0.0f), frameMatrix);
	loc.rot = VTransform(VGet(0.0f, 0.0f, 0.0f), frameMatrix);

	MV1SetMatrix(m_objHandle[loc.name], frameMatrix);
}