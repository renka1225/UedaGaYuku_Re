#include "DxLib.h"
#include "Input.h"
#include "Sound.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cassert>

Sound* Sound::m_instance = nullptr;

namespace
{
	const char* kCsvFilePath = "data/csv/soundData.csv";	// 読み込むcsvファイルのパス
	const std::string kBgmFilePath = "data/sound/BGM/";		// BGMのパス
	const std::string kSeFilePath = "data/sound/SE/";		// SEのパス

	constexpr int kMaxVolumePal = 255; // 最大音量

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
	// すでに再生中の場合は飛ばす
	if (CheckSoundMem(m_seData[seName].handle)) return;

	PlaySoundMem(m_seData[seName].handle, DX_PLAYTYPE_BACK);
	SetBgmVol(seName);
}

void Sound::PlayBgm(std::string bgmName)
{
	// すでに再生中の場合は飛ばす
	if (CheckSoundMem(m_bgmData[bgmName].handle)) return;

	PlaySoundMem(m_bgmData[bgmName].handle, DX_PLAYTYPE_BACK);
	SetBgmVol(bgmName);
}

void Sound::PlayLoopBgm(std::string bgmName)
{
	// すでに再生中の場合は飛ばす
	if (CheckSoundMem(m_bgmData[bgmName].handle)) return;

	PlaySoundMem(m_bgmData[bgmName].handle, DX_PLAYTYPE_LOOP);
	SetBgmVol(bgmName);
}

void Sound::StopBgm(std::string bgmName)
{
	StopSoundMem(m_bgmData[bgmName].handle);
}

void Sound::StopSe(std::string seName)
{
	StopSoundMem(m_seData[seName].handle);
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

void Sound::ChangeBgmVol(Input& input)
{
	for (auto& [name, data] : m_bgmData)
	{
		// 音量を下げる
		if (input.IsPressing(InputId::kLeft))
		{
			m_bgmData[name].vol--;
		}
		// 音量を上げる
		else if (input.IsPressing(InputId::kRight))
		{
			m_bgmData[name].vol++;
		}

		m_bgmData[name].vol = std::clamp(m_bgmData[name].vol, 0, kMaxVolumePal);
		ChangeVolumeSoundMem(m_bgmData[name].vol, data.handle);
	}
}

void Sound::ChangeSeVol(Input& input)
{
	for (auto& [name, data] : m_seData)
	{
		// 音量を下げる
		if (input.IsPressing(InputId::kLeft))
		{
			m_seData[name].vol--;
		}
		// 音量を上げる
		else if (input.IsPressing(InputId::kRight))
		{
			m_seData[name].vol++;
		}

		m_seData[name].vol = std::clamp(m_seData[name].vol, 0, kMaxVolumePal);
		ChangeVolumeSoundMem(m_seData[name].vol, data.handle);
	}
}

int Sound::ChangeVolRate(int vol)
{
	return static_cast<int>((static_cast<float>(vol) / kMaxVolumePal) * 100);
}

int Sound::RestoreVolRate(int vol)
{
	printfDx("%d\n", static_cast<int>((static_cast<float>(vol) / 100.0f) * kMaxVolumePal));
	return static_cast<int>((static_cast<float>(vol) / 100.0f) * kMaxVolumePal);
}

int Sound::GetBgmVol()
{
	// 仮のBGM音量を取得
	return ChangeVolRate(m_bgmData["sampleBGM.mp3"].vol);
}

int Sound::GetSeVol()
{
	// 仮のSE音量を取得
	return ChangeVolRate(m_seData["sampleSE.mp3"].vol);
}

int Sound::GetBgmVol(std::string bgmName)
{
	auto found = m_bgmData.find(bgmName);
	if (found != m_bgmData.end())
	{
		return found->second.vol;
	}

	return 0;
}

int Sound::GetSeVol(std::string seName)
{
	auto found = m_seData.find(seName);
	if (found != m_seData.end())
	{
		return found->second.vol;
	}

	return 0;
}

bool Sound::GetIsPlaySe(std::string seName)
{
	return CheckSoundMem(m_seData[seName].handle);
}

bool Sound::GetIsPlayBgm(std::string bgmName)
{
	return CheckSoundMem(m_bgmData[bgmName].handle);
}
