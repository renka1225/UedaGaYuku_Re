﻿#include "Game.h"
#include "Vec2.h"
#include "ConversionTime.h"
#include "Font.h"
#include "Camera.h"
#include "SaveData.h"
#include <fstream>

SaveData* SaveData::m_instance = nullptr;

namespace
{
	const char* const kSaveDataPath = "data/saveData/savedata."; // セーブデータの保存を行うパス名
	constexpr float kSaveDataInterval = 212.0f;					 // セーブデータ情報表示位置間隔

	const VECTOR kInitPos = VGet(8759.0f, 40.0f, 3109.0f);	// プレイヤーの初期位置
	constexpr float kInitHp = 10000.0f;						// プレイヤーの初期HP
	constexpr int kMaxPossession = 12;						// アイテムの最大所持数

	constexpr float kInitAngleH = -0.6f;					// カメラの初期平行角度
	constexpr float kInitAngleV = -0.3f;					// カメラの初期垂直角度
	constexpr float kHeight = 23.0f;						// カメラの注視点
}

void SaveData::Load(int slot)
{
	// 所持アイテムのサイズ確保
	m_saveData.possessItem.resize(kMaxPossession, -1);
	
	// ファイル読み込み
	std::fstream file;
	std::string saveFilePath = GetSaveDataPath(slot);
	file.open(saveFilePath, std::ios::in | std::ios::binary);

	// ファイル読み込み成功
	if (file.is_open())
	{
		// プレイヤー情報の読み込み
		file.read((char*)&m_saveData.playerPos, sizeof(m_saveData.playerPos));
		file.read((char*)&m_saveData.hp, sizeof(m_saveData.hp));
		file.read((char*)&m_saveData.gauge, sizeof(m_saveData.gauge));
		file.read((char*)&m_saveData.money, sizeof(m_saveData.money));
		file.read((char*)&m_saveData.enhanceStep, sizeof(m_saveData.enhanceStep));
		file.read((char*)&m_saveData.deadEnemyNum, sizeof(m_saveData.deadEnemyNum));

		// 所持アイテムの読み込み
		for (int i = 0; i < m_saveData.possessItem.size(); i++)
		{
			if (!file.read((char*)&m_saveData.possessItem[i], sizeof(m_saveData.possessItem[i])))
			{
				m_saveData.possessItem[i] = -1; // アイテム情報がない場合は-1を代入
			}
		}

		// カメラ情報の読み込み
		file.read((char*)&m_saveData.cameraPos, sizeof(m_saveData.cameraPos));
		file.read((char*)&m_saveData.target, sizeof(m_saveData.target));
		file.read((char*)&m_saveData.angleH, sizeof(m_saveData.angleH));
		file.read((char*)&m_saveData.angleV, sizeof(m_saveData.angleV));

		// 時間情報を読み込む
		file.read((char*)&m_saveData.date.Year, sizeof(m_saveData.date.Year));
		file.read((char*)&m_saveData.date.Mon, sizeof(m_saveData.date.Mon));
		file.read((char*)&m_saveData.date.Day, sizeof(m_saveData.date.Day));
		file.read((char*)&m_saveData.date.Hour, sizeof(m_saveData.date.Hour));
		file.read((char*)&m_saveData.date.Min, sizeof(m_saveData.date.Min));
		file.read((char*)&m_saveData.date.Sec, sizeof(m_saveData.date.Sec));
		file.read((char*)&m_saveData.playTime, sizeof(m_saveData.playTime));

		// 終了状態を読み込む
		file.read((char*)&m_saveData.isEndTutorial, sizeof(m_saveData.isEndTutorial));
		file.read((char*)&m_saveData.isDestoryBob, sizeof(m_saveData.isDestoryBob));
		file.read((char*)&m_saveData.isDestorySato, sizeof(m_saveData.isDestorySato));
		file.read((char*)&m_saveData.isDestoryAbe, sizeof(m_saveData.isDestoryAbe));
		
		file.close();
	}
	else
	{
		// 新しいセーブデータを作る
		CreateNewData(slot);
#ifdef _DEBUG
		printfDx("ファイル新規作成\n");
#endif
	}
}

void SaveData::Write(int slot)
{
	// ファイルに書き込む
	std::ofstream file;
	std::string saveFilePath = GetSaveDataPath(slot);
	file.open(saveFilePath, std::ios::binary);

	// ファイル読み込み成功
	if (file.is_open())
	{
		// プレイヤー情報を書き込む
		file.write((char*)&m_saveData.playerPos, sizeof(m_saveData.playerPos));
		file.write((char*)&m_saveData.hp, sizeof(m_saveData.hp));
		file.write((char*)&m_saveData.gauge, sizeof(m_saveData.gauge));
		file.write((char*)&m_saveData.money, sizeof(m_saveData.money));
		file.write((char*)&m_saveData.enhanceStep, sizeof(m_saveData.enhanceStep));
		file.write((char*)&m_saveData.deadEnemyNum, sizeof(m_saveData.deadEnemyNum));

		// 所持アイテムを書き込む
		for (int i = 0; i < m_saveData.possessItem.size(); i++)
		{
			file.write((char*)&m_saveData.possessItem[i], sizeof(m_saveData.possessItem[i]));
#ifdef _DEBUG
			printfDx("%d\n", m_saveData.possessItem[i]);
#endif
		}

		// カメラ情報を書き込む
		file.write((char*)&m_saveData.cameraPos, sizeof(m_saveData.cameraPos));
		file.write((char*)&m_saveData.target, sizeof(m_saveData.target));
		file.write((char*)&m_saveData.angleH, sizeof(m_saveData.angleH));
		file.write((char*)&m_saveData.angleV, sizeof(m_saveData.angleV));

		// 時間情報を書き込む
		file.write((char*)&m_saveData.date.Year, sizeof(m_saveData.date.Year));
		file.write((char*)&m_saveData.date.Mon, sizeof(m_saveData.date.Mon));
		file.write((char*)&m_saveData.date.Day, sizeof(m_saveData.date.Day));
		file.write((char*)&m_saveData.date.Hour, sizeof(m_saveData.date.Hour));
		file.write((char*)&m_saveData.date.Min, sizeof(m_saveData.date.Min));
		file.write((char*)&m_saveData.date.Sec, sizeof(m_saveData.date.Sec));
		file.write((char*)&m_saveData.playTime, sizeof(m_saveData.playTime));

		// 終了状態を書き込む
		file.write((char*)&m_saveData.isEndTutorial, sizeof(m_saveData.isEndTutorial));
		file.write((char*)&m_saveData.isDestoryBob, sizeof(m_saveData.isDestoryBob));
		file.write((char*)&m_saveData.isDestorySato, sizeof(m_saveData.isDestorySato));
		file.write((char*)&m_saveData.isDestoryAbe, sizeof(m_saveData.isDestoryAbe));

		file.close();
	}
	else
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
}

void SaveData::DeleteData(int slot)
{
	std::remove(kSaveDataPath); // ファイルを削除
}

void SaveData::CreateNewData(int slot)
{
	// プレイヤーデータを作成
	m_saveData.playerPos = kInitPos;
	m_saveData.hp = kInitHp;
	m_saveData.gauge = 0.0f;
	m_saveData.money = 0;
	m_saveData.deadEnemyNum = 0;

	/*強化段階をリセット*/
	m_saveData.isEndTutorial = false;
	m_saveData.enhanceStep.nowAtkUpStep = 0;
	m_saveData.enhanceStep.nowGaugeUpStep = 0;
	m_saveData.enhanceStep.nowHpUpStep = 0;

	// アイテムデータを作成
	m_saveData.possessItem.resize(kMaxPossession);
	for (auto& item : m_saveData.possessItem)
	{
		item = -1;
	}

	// カメラデータを作成
	m_saveData.cameraPos = VGet(0.0f, kHeight, 0.0f);
	m_saveData.target = VGet(0.0f, 0.0f, 0.0f);
	m_saveData.angleH = kInitAngleH;
	m_saveData.angleV = kInitAngleV;

	// 時間データを作成
	SaveDateData();
	m_saveData.playTime = 0;

	m_saveData.isEndTutorial = false;
	m_saveData.isDestoryBob = false;
	m_saveData.isDestorySato = false;
	m_saveData.isDestoryAbe = false;

	Write(slot); // 初期データを保存する
}

void SaveData::WriteData(const Player& pPlayer, const Camera& pCamera, int playTime, int slot)
{
	// プレイヤー情報
	m_saveData.playerPos = pPlayer.GetPos();
	m_saveData.hp = pPlayer.GetHp();
	m_saveData.gauge = pPlayer.GetGauge();
	m_saveData.money = pPlayer.GetMoney();
	m_saveData.enhanceStep = pPlayer.GetEnhanceStep();
	m_saveData.deadEnemyNum = pPlayer.GetDeadEnemyNum();

	m_saveData.possessItem = pPlayer.GetPossessItem();

	// カメラ情報
	m_saveData.cameraPos = pCamera.GetPos();
	m_saveData.target = pCamera.GetAngle();
	m_saveData.angleH = pCamera.GetAngleH();
	m_saveData.angleV = pCamera.GetAngleV();

	// 時間データ
	SaveDateData();
	m_saveData.playTime = playTime;

	// 終了状態
	m_saveData.isEndTutorial = pPlayer.GetTutoInfo().isEndTutorial;
	m_saveData.isDestoryBob = pPlayer.GetDestroySpecialEnemy().isBob;
	m_saveData.isDestorySato = pPlayer.GetDestroySpecialEnemy().isSato;
	m_saveData.isDestoryAbe = pPlayer.GetDestroySpecialEnemy().isAbe;

	Write(slot); // データを上書きする
}

void SaveData::SaveDateData()
{
	DATEDATA date;
	GetDateTime(&date);
	m_saveData.date.Year = date.Year;
	m_saveData.date.Mon = date.Mon;
	m_saveData.date.Day = date.Day;
	m_saveData.date.Hour = date.Hour;
	m_saveData.date.Min = date.Min;
	m_saveData.date.Sec = date.Sec;
}

void SaveData::DrawDateSaveData(Vec2 pos)
{
	auto& saveData = SaveData::GetInstance();

	for (int i = 0; i < static_cast<float>(SaveData::SelectSaveData::kSaveNum); i++)
	{
		saveData.Load(i);

		DrawFormatStringFToHandle(pos.x, pos.y + (kSaveDataInterval * static_cast<float>(i)),
			Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kSave)],
			"%d/%d/%d %d時:%d分:%d秒", m_saveData.date.Year, m_saveData.date.Mon, m_saveData.date.Day, m_saveData.date.Hour, m_saveData.date.Min, m_saveData.date.Sec);
	}
}

void SaveData::DrawPlayTimeSaveData(Vec2 pos)
{
	auto& saveData = SaveData::GetInstance();

	for (int i = 0; i < static_cast<float>(SaveData::SelectSaveData::kSaveNum); i++)
	{
		saveData.Load(i);
		int playTime = m_saveData.playTime;

		DrawFormatStringFToHandle(pos.x, pos.y + (kSaveDataInterval * static_cast<float>(i)),
		Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kSave)],
		"プレイ時間 %d時間:%d分:%d秒", Conversion::ChangeHour(playTime), Conversion::ChangeMin(playTime), Conversion::ChangeSec(playTime));
	}
}

std::string SaveData::GetSaveDataPath(int slot)
{
	return kSaveDataPath + std::to_string(slot) + ".dat";
}
