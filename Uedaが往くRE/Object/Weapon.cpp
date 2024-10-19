#include "DebugDraw.h"
#include "LoadCsv.h"
#include "Weapon.h"

// 定数
namespace
{
	const std::string kWeaponFileName = "data/model/weapon/"; // モデルのファイルパス名
	constexpr float kGravity = -15.0f; // 重力
}

Weapon::Weapon():
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
	for (const auto& loc : m_locationData)
	{
		LoadCsv::GetInstance().LoadWeaponData(m_weaponData, loc.name);
		m_durability = m_weaponData.durability;
		MV1SetScale(m_objHandle[loc.name], loc.scale);
		MV1SetPosition(m_objHandle[loc.name], loc.pos);
	}
}

void Weapon::Update()
{
}

void Weapon::Draw()
{
	for (const auto& loc : m_locationData)
	{
		MV1DrawModel(m_objHandle[loc.name]); // モデル表示
	}

#ifdef _DEBUG
	DebugDraw debug;
	// 当たり判定描画
	debug.DrawWeaponCol(m_weaponData.colStartPos, m_weaponData.colEndPos, m_weaponData.colRadius);

	for (const auto& loc : m_locationData)
	{
		debug.DrawWeaponInfo(loc.name.c_str(), loc.tag.c_str(), loc.pos, loc.rot, loc.scale, m_durability);
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
		FileRead_read(loc.name.data(), sizeof(char) * loc.name.size(), m_locationDataHandle);

		// タグをロード
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), m_locationDataHandle);
		loc.tag.resize(tagCnt);
		FileRead_read(loc.tag.data(), sizeof(char) * loc.tag.size(), m_locationDataHandle);

		// 座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), m_locationDataHandle);
		// 回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), m_locationDataHandle);
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