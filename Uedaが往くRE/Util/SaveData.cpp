#include "SaveData.h"
#include <fstream>

SaveData* SaveData::m_instance = nullptr;

namespace
{
	constexpr int kCurrentSaveVersion = 0;				   // セーブデータのバージョン
	const char* const kSaveDataPath = "data/savedata.dat"; // セーブデータの保存を行うパス名

	const VECTOR kInitPos = VGet(7425.0f, 40.0f, 5190.0f);	// プレイヤーの初期位置
}

void SaveData::Load()
{
	std::fstream file;
	file.open(kSaveDataPath, std::ios::in | std::ios::binary);

	// ファイル読み込み成功
	if (file.is_open())
	{
		file.read((char*)&m_saveData, sizeof(m_saveData)); // セーブデータを読み込む
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
		file.write((char*)&m_saveData, sizeof(m_saveData)); // セーブデータを書き込む
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
	m_saveData.playTime = 0;

	Write(); // 初期データを保存する
}
