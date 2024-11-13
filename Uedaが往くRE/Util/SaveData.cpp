#include "SaveData.h"
#include <fstream>

SaveData* SaveData::m_instance = nullptr;

namespace
{
	constexpr int kCurrentSaveVersion = 0;				   // セーブデータのバージョン
	const char* const kSaveDataPath = "data/savedata.dat"; // セーブデータの保存を行うパス名
}

void SaveData::Load()
{
	std::fstream file;
	file.open(kSaveDataPath, std::ios::in | std::ios::binary);
	if (!file.is_open()) // ファイル読み込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル読み込み失敗\n");
#endif
	}
	else // ファイル読み込み成功
	{
		// セーブデータを作成する
		CreateNewData();
		file.close();
	}
}

void SaveData::Write()
{
	// ファイルに書き込む
	std::fstream file;
	file.open(kSaveDataPath, std::ios::out | std::ios::binary);
	if (!file.is_open()) // ファイル読み込み失敗時
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
	else	// ファイル読み込み成功
	{
		file.close();
	}
}

void SaveData::DeleteData()
{

}

void SaveData::CreateNewData()
{
}
