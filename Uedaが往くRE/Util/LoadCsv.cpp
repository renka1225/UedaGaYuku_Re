#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFileName = "data/csv/charaStatus.csv";
	const char* const kCharaAnimDataFileName = "data/csv/animData.csv";

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
void LoadCsv::LoadStatus(CharacterBase::Status& data, const char* charaName)
{
	std::ifstream ifs(kCharaStatusFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// ステータス情報を代入する
	data.maxHp = std::stof(strvec[1]);
	data.walkSpeed = std::stof(strvec[2]);
	data.runSpeed = std::stof(strvec[3]);
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
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();
	}

	// アニメーション情報を設定
	std::string animName = strvec[0];
	data[animName].number = std::stoi(strvec[1]);
	data[animName].loopFrame = std::stof(strvec[2]);
	data[animName].endFrame = std::stof(strvec[3]);
	data[animName].playSpeed = std::stof(strvec[4]);
}
