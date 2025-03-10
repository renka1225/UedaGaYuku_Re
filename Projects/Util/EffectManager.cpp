﻿#include "EffekseerForDXLib.h"
#include "Input.h"
#include "ModelFrameName.h"
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
			data.effectHandle = LoadEffekseerEffect((kEffectFilePath + strvec[0] + ".efk").c_str());
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
			continue;
		}

		m_effectData[strvec[0]] = data;
	}
}

void EffectManager::UnLoad()
{
	m_effects.clear();
	m_weaponEffects.clear();
}

void EffectManager::Delete(const std::string& name)
{
	auto it = m_effectData.find(name);
	if (it != m_effectData.end())
	{
		m_effectData.erase(it);
	}
}

void EffectManager::AllStop()
{
	for (auto& data : m_effects)
	{
		if (data.isPlaying)
		{
			StopEffekseer3DEffect(data.playingHandle);
			data.isPlaying = false;
		}
	}

	for (auto& pair : m_weaponEffects)
	{
		EffectData& data = pair.second;
		if (data.isPlaying)
		{
			StopEffekseer3DEffect(data.playingHandle);
			data.isPlaying = false;
		}
	}
}

void EffectManager::Init()
{
	// フルスクリーンウィンドウの切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	// DXライブラリのデバイスロストした時のコールバックを設定する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	// エフェクトの歪みを適用する
	Effekseer_InitDistortion();
}

void EffectManager::Update()
{
	Effekseer_Sync3DSetting();	// 3Dの情報をDxLibとEffekseerで合わせる
	UpdateEffekseer3D();

	for (auto it = m_effects.begin(); it != m_effects.end();)
	{
		EffectData& data = *it;

		// プレイヤーに追従させる
		if (data.pPlayer)
		{
			data.pos = data.pPlayer->GetModelFramePos(PlayerFrameName::kRightEnd.c_str());
		}

		if (!data.isPlaying)
		{
			it = m_effects.erase(it); // エフェクトを削除する
#ifdef _DEBUG
			printfDx("エフェクト削除\n");
#endif
			continue;
		}

		data.elapsedTime++;

		// 再生時間を超えた場合
		if (data.elapsedTime >= data.playTime)
		{
			// ループ再生の場合
			if (data.isLoop)
			{
				data.elapsedTime = 0;
				data.playingHandle = PlayEffekseer3DEffect(data.effectHandle);
			}
			else
			{
				StopEffekseer3DEffect(data.playingHandle);
				data.isPlaying = false;
			}
		}
		else
		{
			SetPosPlayingEffekseer3DEffect(data.playingHandle, data.pos.x, data.pos.y, data.pos.z);
			SetScalePlayingEffekseer3DEffect(data.playingHandle, data.scale, data.scale, data.scale);
			SetRotationPlayingEffekseer3DEffect(data.playingHandle, data.rotate.x, data.rotate.y, data.rotate.z);
		}

		it++;
	}

	// 武器エフェクト
	for (auto it = m_weaponEffects.begin(); it != m_weaponEffects.end();)
	{
		EffectData& data = it->second;

		// 再生中でない場合
		if (!data.isPlaying)
		{
			StopEffekseer3DEffect(data.playingHandle);
			//continue;
		}

		data.elapsedTime++;
		// 再生時間を超えた場合
		if (data.elapsedTime >= data.playTime)
		{
			// ループ再生
			data.elapsedTime = 0;
			data.playingHandle = PlayEffekseer3DEffect(data.effectHandle);
		}

		SetPosPlayingEffekseer3DEffect(data.playingHandle, data.pos.x, data.pos.y, data.pos.z);
		SetScalePlayingEffekseer3DEffect(data.playingHandle, data.scale, data.scale, data.scale);
		SetRotationPlayingEffekseer3DEffect(data.playingHandle, data.rotate.x, data.rotate.y, data.rotate.z);

		it++;
	}
}

void EffectManager::Draw()
{
	DrawEffekseer3D();
}

void EffectManager::Add(const std::string& name, const VECTOR& pos, float adjust, Player* pPlayer)
{
	auto it = m_effectData.find(name);
	if (it != m_effectData.end())
	{
		EffectData& data = it->second;

		data.pPlayer = pPlayer;

		// エフェクトの位置を調整
		data.pos = VAdd(data.adjPos, pos);
		data.pos.y += adjust;

		data.elapsedTime = 0;
		data.isPlaying = true;
		data.playingHandle = PlayEffekseer3DEffect(data.effectHandle);

		if (name == EffectName::kItemDrop)
		{
			data.isLoop = true;
		}

		m_effects.push_back(data);
	}
}

void EffectManager::AddWeaponEffect(const std::string id, const VECTOR& pos)
{
	// すでに追加されている場合飛ばす
	if (m_weaponEffects.find(id) != m_weaponEffects.end()) return;

	auto it = m_effectData.find(EffectName::kWeapon);
	if (it == m_effectData.end()) return;

	EffectData data = it->second;
	data.isPlaying = true;
	data.isLoop = true;
	data.pos = pos;
	data.playingHandle = PlayEffekseer3DEffect(data.effectHandle);

	m_weaponEffects[id] = data;
}

void EffectManager::StopWeaponEffect(const std::string id)
{
	auto it = m_weaponEffects.find(id);
	if (it != m_weaponEffects.end())
	{
		StopEffekseer3DEffect(it->second.playingHandle);
		it->second.isPlaying = false;
		m_weaponEffects.erase(it);
	}
}

void EffectManager::StopItemEffect(const std::string& name, const VECTOR& pos)
{
	for (auto it = m_effects.begin(); it != m_effects.end();)
	{
		EffectData& data = *it;

		// 位置が一致するかどうか
		if (VSize(pos) == VSize(data.pos))
		{
			StopEffekseer3DEffect(data.playingHandle);
			data.isPlaying = false;
			it = m_effects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool EffectManager::GetIsPlayingWeaponEffect(const std::string& id)
{
	auto it = m_weaponEffects.find(id);
	return (it != m_weaponEffects.end() && it->second.isPlaying);
}
