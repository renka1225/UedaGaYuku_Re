#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFilePath = "data/csv/charaStatus.csv";	// キャラクターステータス
	const char* const kCharaColDataFilePath = "data/csv/charaColData.csv";	// キャラクターの当たり判定データ
	const char* const kCharaAnimDataFilePath = "data/csv/animData.csv";		// アニメーションデータ
	const char* const kWeaponDataFilePath = "data/csv/weaponData.csv";		// 武器のデータ
	const char* const kUiDataFilePath = "data/csv/uiData.csv";				// UIデータ
	const char* const kEnemyNameDataFilePath = "data/csv/enemyName.csv";	// 敵の名前データ
	const char* const kMessageFilePath = "data/csv/message.csv";			// メッセージデータ

	constexpr int kEnemyNamekind = 10;	// 敵名の種類

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
	std::ifstream ifs(kCharaStatusFilePath);
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
				data.avoidDist = std::stof(strvec[4]);
				data.atkPowerPunch1 = std::stof(strvec[5]);
				data.atkPowerPunch2 = std::stof(strvec[6]);
				data.atkPowerPunch3 = std::stof(strvec[7]);
				data.atkPowerKick = std::stof(strvec[8]);
				data.atkPowerOneHandWeapon = std::stof(strvec[9]);
				data.atkPowerTwoHandWeapon = std::stof(strvec[10]);
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
	std::ifstream ifs(kCharaColDataFilePath);
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
				data.bodyRadius = std::stof(strvec[7]);
				data.armRadius = std::stof(strvec[8]);
				data.legRadius = std::stof(strvec[9]);
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
	std::ifstream ifs(kCharaAnimDataFilePath);
	std::string line;
	std::vector<std::string> strvec;

    while (std::getline(ifs, line))
    {
        strvec = split(line, ',');

        // アニメーション情報を設定
        std::string animName = strvec[0];
        try
		{
			data[animName].loopFrame = std::stof(strvec[1]);
			data[animName].playSpeed = std::stof(strvec[2]);
        }
        catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
        }
    }
}

void LoadCsv::LoadWeaponData(Weapon::WeaponData& data, std::string weaponName)
{
	std::ifstream ifs(kWeaponDataFilePath);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		std::string charaId = strvec[0].c_str();

		if (charaId == weaponName)
		{
			try
			{
				// 外部ファイルの情報を入れる
				data.durability = std::stoi(strvec[1]);
				data.colStartPos.x = std::stof(strvec[2]);
				data.colStartPos.y = std::stof(strvec[3]);
				data.colStartPos.z = std::stof(strvec[4]);
				data.colEndPos.x = std::stof(strvec[5]);
				data.colEndPos.y = std::stof(strvec[6]);
				data.colEndPos.z = std::stof(strvec[7]);
				data.colRadius = std::stof(strvec[8]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}

			break;
		}
	}
}

void LoadCsv::LoadUiData(UiBase::UiData& data)
{
	std::ifstream ifs(kUiDataFilePath);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');

		try
		{
			data.LTposX = std::stof(strvec[1]);
			data.LTposY = std::stof(strvec[2]);
			data.RBposX = std::stof(strvec[3]);
			data.RBposY = std::stof(strvec[4]);
			data.scale = std::stof(strvec[5]);
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
	std::ifstream ifs(kMessageFilePath);
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

void LoadCsv::LoadEnemyName()
{
	std::ifstream ifs(kEnemyNameDataFilePath);
	std::string line;
	std::vector<std::string> strvec;

	for (int i = 0; i < kEnemyNamekind; i++)
	{
		while (std::getline(ifs, line))
		{
			std::vector<std::string> strvec = split(line, ',');
			if (!strvec.empty())
			{
				m_enemyNameData.push_back(strvec[i]);
			}
		}
	}
}

std::string LoadCsv::GetEnemyName(int enemyIndex)
{
	return m_enemyNameData[enemyIndex];
}
