#include "Camera.h"
#include "SaveData.h"
#include <fstream>

SaveData* SaveData::m_instance = nullptr;

namespace
{
	constexpr int kCurrentSaveVersion = 0;				   // セーブデータのバージョン
	const char* const kSaveDataPath = "data/savedata.dat"; // セーブデータの保存を行うパス名

	const VECTOR kInitPos = VGet(7425.0f, 40.0f, 5190.0f);	// プレイヤーの初期位置
	constexpr int kMaxPossession = 12;		// アイテムの最大所持数

	constexpr float kInitAngleH = -0.6f;					// カメラの初期平行角度
	constexpr float kInitAngleV = -0.3f;					// カメラの初期垂直角度
	constexpr float kHeight = 23.0f;						// カメラの注視点
}

void SaveData::Load()
{
	std::fstream file;
	file.open(kSaveDataPath, std::ios::in | std::ios::binary);

	// ファイル読み込み成功
	if (file.is_open())
	{
		file.read((char*)&m_saveData, sizeof(SaveDataCore)); // セーブデータを読み込む

#ifdef _DEBUG
		printfDx("ファイル読み込み成功\n");
#endif

		file.close();
	}
	// ファイル読み込み失敗
	else
	{
		// 新しいセーブデータを作る
		CreateNewData();
#ifdef _DEBUG
		printfDx("ファイル新規作成\n");
#endif
	}
}

void SaveData::Write()
{
	// ファイルに書き込む
	std::ofstream file;
	file.open(kSaveDataPath, std::ios::binary);

	// ファイル読み込み成功
	if (file.is_open())
	{
		file.write((char*)&m_saveData, sizeof(SaveDataCore)); // セーブデータを書き込む

#ifdef _DEBUG
		printfDx("ファイル書き込み成功\n");
#endif

		file.close();
	}
	// ファイル読み込み失敗
	else
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
}

void SaveData::DeleteData()
{
	std::remove(kSaveDataPath); // ファイルを削除
}

void SaveData::CreateNewData()
{
	m_saveData.playerPos = kInitPos;
	m_saveData.hp = 100.0f;
	m_saveData.gauge = 0.0f;
	m_saveData.money = 0;

	m_saveData.cameraPos = VGet(0.0f, kHeight, 0.0f);
	m_saveData.target = VGet(0.0f, 0.0f, 0.0f);
	m_saveData.angleH = kInitAngleH;
	m_saveData.angleV = kInitAngleV;

	Write(); // 初期データを保存する
}

void SaveData::SetPlayerData(const Player& pPlayer)
{
	m_saveData.playerPos = pPlayer.GetPos();
	m_saveData.hp = pPlayer.GetHp();
	m_saveData.gauge = pPlayer.GetGauge();
	m_saveData.money = pPlayer.GetMoney();
	m_saveData.enhanceStep = pPlayer.GetEnhanceStep();

	//m_saveData.possessItem.resize(kMaxPossession, -1);
	//for (int i = 0; i < m_saveData.possessItem.size(); i++)
	//{
	//	m_saveData.possessItem[i] = pPlayer.GetPossessItem()[i];
	//}
	auto item = pPlayer.GetPossessItem();

	for (int i = 0; i < m_saveData.possessItem.size(); i++)
	{
		m_saveData.possessItem[i] = item[i];
		printfDx("%d", m_saveData.possessItem[i]);
	}

	//std::vector<int> item = pPlayer.GetPossessItem();
	//m_saveData.possessItem.assign(item.begin(), item.end());

	Write(); // データを上書きする
}

void SaveData::SetCameraData(const Camera& pCamera)
{
	m_saveData.cameraPos = pCamera.GetPos();
	m_saveData.target = pCamera.GetAngle();
	m_saveData.angleH = pCamera.GetAngleH();
	m_saveData.angleV = pCamera.GetAngleV();
}
