﻿#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFileName = "data/csv/charaStatus.csv";	// キャラクターステータス
	const char* const kCharaAnimDataFileName = "data/csv/animData.csv";		// アニメーションデータ
	const char* const kColDataFileName = "data/csv/collisionData.csv";		// 当たり判定データ
	const char* const kUiDataFileName = "data/csv/uiData.csv";				// UIデータ
	const char* const kMessageFileName = "data/csv/message.csv";			// メッセージデータ

	constexpr int kStatusNum = 4;	// ステータスの情報数
	constexpr int kColNum = 23;		// 当たり判定の情報数

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void LoadCsv::LoadStatus(CharacterBase::Status& data, std::string charaName)
{
	std::ifstream ifs(kCharaStatusFileName);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* charaId = strvec[0].c_str();

		if (charaId == charaName)
		{
			try
			{
				// ステータス情報を代入する
				data.maxHp = std::stof(strvec[1]);
				data.walkSpeed = std::stof(strvec[2]);
				data.runSpeed = std::stof(strvec[3]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}
}

void LoadCsv::LoadColData(CharacterBase::ColData& data, std::string charaName)
{
	std::ifstream ifs(kColDataFileName);
	std::string line;
	std::vector<std::string> strvec;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		std::string charaId = strvec[0].c_str();

		if (charaId == charaName)
		{
			try
			{
				// 外部ファイルの情報を入れる
				data.bodyStartPos.x = std::stof(strvec[1]);
				data.bodyStartPos.y = std::stof(strvec[2]);
				data.bodyStartPos.z = std::stof(strvec[3]);
				data.bodyEndPos.x = std::stof(strvec[4]);
				data.bodyEndPos.y = std::stof(strvec[5]);
				data.bodyEndPos.z = std::stof(strvec[6]);
				data.armStartPos.x = std::stof(strvec[7]);
				data.armStartPos.y = std::stof(strvec[8]);
				data.armStartPos.z = std::stof(strvec[9]);
				data.armEndPos.x = std::stof(strvec[10]);
				data.armEndPos.y = std::stof(strvec[11]);
				data.armEndPos.z = std::stof(strvec[12]);
				data.legStartPos.x = std::stof(strvec[13]);
				data.legStartPos.y = std::stof(strvec[14]);
				data.legStartPos.z = std::stof(strvec[15]);
				data.legEndPos.x = std::stof(strvec[16]);
				data.legEndPos.y = std::stof(strvec[17]);
				data.legEndPos.z = std::stof(strvec[18]);
				data.bodyRadius = std::stof(strvec[19]);
				data.aimRadius = std::stof(strvec[20]);
				data.legRadius = std::stof(strvec[21]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}

			break;
		}
	}
}

void LoadCsv::LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kCharaAnimDataFileName);
	std::string line;
	std::vector<std::string> strvec;

    while (std::getline(ifs, line))
    {
        strvec = split(line, ',');

        // アニメーション情報を設定
        std::string animName = strvec[0];
        try
		{
			data[animName].number = std::stoi(strvec[1]);
			data[animName].loopFrame = std::stof(strvec[2]);
			data[animName].endFrame = std::stof(strvec[3]);
			data[animName].playSpeed = std::stof(strvec[4]);
        }
        catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
        }
    }
}

void LoadCsv::LoadUiData(UiBase::UiData& data)
{
	std::ifstream ifs(kUiDataFileName);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');

		try
		{
			data.posX = std::stof(strvec[1]);
			data.posY = std::stof(strvec[2]);
			data.scale = std::stof(strvec[3]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}

		m_uiData[strvec.at(0)] = data;
	}
}

void LoadCsv::LoadMessage()
{
	std::ifstream ifs(kMessageFileName);
	std::string line;
	m_messageData.clear();

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		m_messageData[strvec.at(0)] = strvec.at(1);
	}
}

UiBase::UiData LoadCsv::GetUiData(std::string id)
{
	return m_uiData[id];
}

std::string LoadCsv::Get_sMessage(std::string id)
{
	return m_messageData[id];
}

const char* LoadCsv::Get_cMessage(std::string id)
{
	return m_messageData[id].c_str();
}
