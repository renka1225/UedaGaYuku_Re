#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFileName = "data/csv/charaStatus.csv";
	const char* const kCharaAnimDataFileName = "data/csv/animData.csv";
	const char* const kColDataFileName = "data/csv/collisionData.csv";

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

/// <summary>
/// キャラクターのステータス情報を読み込む
/// </summary>
/// <param name="data">ステータス情報</param>
/// <param name="charaName">キャラクターの名前</param>
void LoadCsv::LoadStatus(CharacterBase::Status& data, int charaName)
{
	std::ifstream ifs(kCharaStatusFileName);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		try
		{
			// ステータス情報を代入する
			data.maxHp = std::stof(strvec[1 + charaName * kStatusNum]);
			data.walkSpeed = std::stof(strvec[2 + charaName * kStatusNum]);
			data.runSpeed = std::stof(strvec[3 + charaName * kStatusNum]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}
	}
}

/// <summary>
/// 当たり判定のデータを読み込む
/// </summary>
/// <param name="data"></param>
/// <param name="charType"></param>
void LoadCsv::LoadColData(CharacterBase::ColData& data, int charaName)
{
	std::ifstream ifs(kColDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	int charNum = charaName * kColNum;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		try
		{

			// 外部ファイルの情報を入れる
			data.bodyStartPos.x = std::stof(strvec[1 + charNum]);
			data.bodyStartPos.y = std::stof(strvec[2 + charNum]);
			data.bodyStartPos.z = std::stof(strvec[3 + charNum]);
			data.bodyEndPos.x = std::stof(strvec[4 + charNum]);
			data.bodyEndPos.y = std::stof(strvec[5 + charNum]);
			data.bodyEndPos.z = std::stof(strvec[6 + charNum]);
			data.armStartPos.x = std::stof(strvec[7 + charNum]);
			data.armStartPos.y = std::stof(strvec[8 + charNum]);
			data.armStartPos.z = std::stof(strvec[9 + charNum]);
			data.armEndPos.x = std::stof(strvec[10 + charNum]);
			data.armEndPos.y = std::stof(strvec[11 + charNum]);
			data.armEndPos.z = std::stof(strvec[12 + charNum]);
			data.legStartPos.x = std::stof(strvec[13 + charNum]);
			data.legStartPos.y = std::stof(strvec[14 + charNum]);
			data.legStartPos.z = std::stof(strvec[15 + charNum]);
			data.legEndPos.x = std::stof(strvec[16 + charNum]);
			data.legEndPos.y = std::stof(strvec[17 + charNum]);
			data.legEndPos.z = std::stof(strvec[18 + charNum]);
			data.bodyRadius = std::stof(strvec[19 + charNum]);
			data.aimRadius = std::stof(strvec[20 + charNum]);
			data.legRadius = std::stof(strvec[21 + charNum]);
		}
		catch (const std::invalid_argument&)
		{
			// 無効な文字列をスキップ
		}
	}
}


/// <summary>
/// アニメーション情報を読み込む
/// </summary>
/// <param name="data">アニメーション情報</param>
/// <param name="charaName">キャラクターの名前</param>
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