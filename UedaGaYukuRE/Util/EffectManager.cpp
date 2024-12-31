#include "EffekseerForDXLib.h"
#include "Input.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EffectManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

EffectManager* EffectManager::m_instance = nullptr;

// 定数
namespace
{
	const char* kCsvFilePath = "data/csv/effectData.csv";	// 読み込むcsvファイルのパス
	const std::string kEffectFilePath = "data/effect/";		// エフェクトのパス

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

void EffectManager::Load()
{
	std::ifstream ifs(kCsvFilePath);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		EffectData data;

		try
		{
			data.effektHandle = LoadEffekseerEffect((kEffectFilePath + strvec[0] + ".efk").c_str());
			data.adjPos.x = stof(strvec[1]);
			data.adjPos.y = stof(strvec[2]);
			data.adjPos.z = stof(strvec[3]);
			data.rotate.x = stof(strvec[4]);
			data.rotate.y = stof(strvec[5]);
			data.rotate.z = stof(strvec[6]);
			data.scale = stof(strvec[7]);
			data.playTime = stoi(strvec[8]);
		}
		catch (const std::invalid_argument&)
		{
		}

		m_effectData[strvec[0]] = data;
	}
}

void EffectManager::Delete(const std::string name)
{
	auto it = m_effectData.find(name);
	if (it != m_effectData.end())
	{
		m_effectData.erase(it);
	}
}

void EffectManager::Init()
{
	Effekseer_InitDistortion();	// エフェクトの歪みを適用する
}

void EffectManager::Update()
{
	Effekseer_Sync3DSetting();	// 3Dの情報をDxLibとEffekseerで合わせる
	UpdateEffekseer3D();

	for (auto& [name, data] : m_effectData)
	{
		if (!data.isPlaying) continue;
		data.elapsedTime++;

		// 再生時間を超えた場合
		if (data.elapsedTime >= data.playTime)
		{
			// ループ再生の場合
			if (data.isLoop)
			{
				data.elapsedTime = 0;
				data.playingHandle = PlayEffekseer3DEffect(data.effektHandle);
			}
			else
			{
				StopEffekseer3DEffect(data.playingHandle); // 再生中のエフェクトを停止する
				data.isPlaying = false;
			}
		}
		else
		{
			SetPosPlayingEffekseer3DEffect(data.playingHandle, data.pos.x, data.pos.y, data.pos.z);
			SetScalePlayingEffekseer3DEffect(data.playingHandle, data.scale, data.scale, data.scale);
			SetRotationPlayingEffekseer3DEffect(data.playingHandle, data.rotate.x, data.rotate.y, data.rotate.z);
		}
	}
}

void EffectManager::Draw()
{
	DrawEffekseer3D();
}

void EffectManager::Add(const std::string name, VECTOR pos)
{
	auto it = m_effectData.find(name);
	if (it != m_effectData.end())
	{
		EffectData& data = it->second;
		data.pos = VAdd(data.adjPos, pos); // エフェクトの位置を調整
		data.elapsedTime = 0;
		data.isPlaying = true;
		data.playingHandle = PlayEffekseer3DEffect(data.effektHandle);

		if (name == EffectName::kItemDrop)
		{
			data.isLoop = true;
		}
	}
}
