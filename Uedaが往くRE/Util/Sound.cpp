#include "DxLib.h"
#include "Sound.h"
#include <fstream>
#include <sstream>

Sound* Sound::m_instance = nullptr;

namespace
{
	const char* kSoundFilePath = "data/csv/soundData.csv";
	const std::string kBgmFilePath = "data/sound/BGM/";
	const std::string kSeFilePath = "data/sound/SE/";

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

void Sound::Load()
{
	//m_bgmData.resize(BgmKind::kBgmNum);
	//m_seData.resize(SeKind::kSeNum);

	std::ifstream ifs(kSoundFilePath);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		SoundData data;

		// BGMの場合
		if (strvec[1] == "BGM")
		{
			data.handle = LoadSoundMem((kBgmFilePath + strvec[0]).c_str());
			data.vol = stoi(strvec[2]);
			m_bgmData[strvec[0]] = data;
		}
		// SEの場合
		else if (strvec[1] == "SE")
		{
			data.handle = LoadSoundMem((kSeFilePath + strvec[0]).c_str());
			data.vol = stoi(strvec[2]);
			m_seData[strvec[0]] = data;
		}
	}
}

void Sound::UnLoad()
{
	for (const auto& data : m_bgmData)
	{
		DeleteSoundMem(data.second.handle);
	}
	for (const auto& data : m_seData)
	{
		DeleteSoundMem(data.second.handle);
	}
}

void Sound::PlaySE(SeKind sekind)
{
	//PlaySoundMem(m_seData.[sekind].handle, DX_PLAYTYPE_BACK);
}

void Sound::PlayBGM(BgmKind bgmKind)
{
	//PlaySoundMem(m_bgmData[bgmKind].handle, DX_PLAYTYPE_BACK);
}

void Sound::SetBgmVol(std::string bgmName)
{
	for (int i = 0; i < m_bgmData.size(); i++)
	{
		m_bgmData[bgmName].vol = std::max(0, m_bgmData[bgmName].vol);
	}
}

void Sound::SetSeVol(std::string seName)
{
	for (int i = 0; i < m_seData.size(); i++)
	{
		m_seData[seName].vol = std::max(0, m_seData[seName].vol);
	}
}
