#include "DxLib.h"
#include "Sound.h"
#include <fstream>
#include <sstream>
#include <cassert>

Sound* Sound::m_instance = nullptr;

namespace
{
	const char* kCsvFilePath = "data/csv/soundData.csv";	// 読み込むcsvファイルのパス
	const std::string kBgmFilePath = "data/sound/BGM/";		// BGMのパス
	const std::string kSeFilePath = "data/sound/SE/";		// SEのパス

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
	std::ifstream ifs(kCsvFilePath);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		SoundData data;

		// BGMの場合
		if (strvec[1] == "BGM")
		{
			data.handle = LoadSoundMem((kBgmFilePath + strvec[0]).c_str());
			assert(data.handle != -1);
			data.vol = stoi(strvec[2]);
			m_bgmData[strvec[0]] = data;
		}
		// SEの場合
		else if (strvec[1] == "SE")
		{
			data.handle = LoadSoundMem((kSeFilePath + strvec[0]).c_str());
			assert(data.handle != -1);
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

void Sound::PlaySe(std::string seName)
{
	PlaySoundMem(m_seData[seName].handle, DX_PLAYTYPE_BACK);
	SetBgmVol(seName);
}

void Sound::PlayBgm(std::string bgmName)
{
	PlaySoundMem(m_bgmData[bgmName].handle, DX_PLAYTYPE_LOOP);
	SetBgmVol(bgmName);
}

void Sound::StopBgm(std::string bgmName)
{
	StopSoundMem(m_bgmData[bgmName].handle);
}

void Sound::SetBgmVol(std::string bgmName)
{
	m_bgmData[bgmName].vol = std::max(0, m_bgmData[bgmName].vol);
	ChangeVolumeSoundMem(m_bgmData[bgmName].vol, m_bgmData[bgmName].handle);
}

void Sound::SetSeVol(std::string seName)
{
	m_seData[seName].vol = std::max(0, m_seData[seName].vol);
	ChangeVolumeSoundMem(m_seData[seName].vol, m_seData[seName].handle);
}

int Sound::GetBgmVol(std::string bgmName)
{
	auto found = m_bgmData.find(bgmName);
	if (found != m_bgmData.end())
	{
		return found->second.vol;
	}
}

int Sound::GetSeVol(std::string seName)
{
	auto found = m_seData.find(seName);
	if (found != m_seData.end())
	{
		return found->second.vol;
	}
}

bool Sound::GetIsPlaySe(std::string seName)
{
	return CheckSoundMem(m_seData[seName].handle);
}

bool Sound::GetIsPlayBgm(std::string bgmName)
{
	return CheckSoundMem(m_bgmData[bgmName].handle);
}
