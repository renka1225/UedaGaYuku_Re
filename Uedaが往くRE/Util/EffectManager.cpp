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
			data.handle = LoadEffekseerEffect((kEffectFilePath + strvec[0]).c_str());
			//assert(data.handle != -1);
			data.pos.x = stof(strvec[1]);
			data.pos.y = stof(strvec[2]);
			data.pos.z = stof(strvec[3]);
			data.scale = stof(strvec[4]);
			data.playTime = stoi(strvec[5]);
		}
		catch (const std::invalid_argument& e)
		{
		}

		m_effectData[strvec[0]] = data;
	}
}

void EffectManager::Delete()
{
	m_effectData.clear();
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
		data.elapsedTime++;

		// 再生時間を超えた場合
		if (data.elapsedTime >= data.playTime)
		{
			StopEffekseer3DEffect(data.handle); // 再生中のエフェクトを停止する
		}
	}
}

void EffectManager::UpdatePos(std::string effectName)
{
	SetPosPlayingEffekseer3DEffect(m_effectData[effectName].handle, m_effectData[effectName].pos.x, m_effectData[effectName].pos.y, m_effectData[effectName].pos.z);
}


void EffectManager::UpdateScale(std::string effectName)
{
	SetScalePlayingEffekseer3DEffect(m_effectData[effectName].handle, m_effectData[effectName].scale, m_effectData[effectName].scale, m_effectData[effectName].scale);
}

void EffectManager::Draw()
{
	DrawEffekseer3D();
}

void EffectManager::Add(const std::string name, const VECTOR enemyPos)
{
	auto it = m_effectData.find(name);
	if (it != m_effectData.end())
	{
		EffectData& data = it->second;
		data.pos = enemyPos;
		data.elapsedTime = 0;

		UpdateScale(name);
		UpdatePos(name);
	}
}
