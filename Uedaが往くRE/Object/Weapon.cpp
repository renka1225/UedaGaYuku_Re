#include "Weapon.h"

// 定数
namespace
{
	const std::string kWeaponFileName = "data/model/Weapon"; // モデルのファイルパス名
	constexpr float kGravity = -15.0f; // 重力
}

Weapon::Weapon():
	m_locationDataHandle(-1)
{
	//LoadLocationData();
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
}

void Weapon::Update()
{
}

void Weapon::Draw()
{
#ifdef _DEBUG
	int y = 300;
	for (const auto& loc : m_locationData)
	{
		DrawFormatString(0, y, 0xffffff, "name:%s, tag:%s, pos(%2.2f:%2.2f:%2.2f), rot(%2.2f:%2.2f:%2.2f), scale(%2.2f:%2.2f:%2.2f)",
			loc.name.c_str(), loc.tag.c_str(), loc.pos.x, loc.pos.y, loc.pos.z, loc.rot.x, loc.rot.y, loc.rot.z, loc.scale.x, loc.scale.y, loc.scale.z);
		y += 20;

		MV1DrawModel(m_objHandle[loc.name]);
	}
#endif
}

void Weapon::LoadLocationData()
{
	m_locationDataHandle = FileRead_open((kWeaponFileName + "modelData.loc").c_str());

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