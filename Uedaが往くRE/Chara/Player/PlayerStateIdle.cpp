﻿#include "Input.h"
#include "LoadCsv.h"
#include "Player.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

/// <summary>
/// 初期化
/// </summary>
void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim("StandIdle");
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力処理</param>
void PlayerStateIdle::Update(const Input& input, const Camera& camera)
{
	// 移動ボタンが押されている場合
	if (input.IsPressing("A") && (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down")))
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
		state->Init();
		return;
	}
	else if (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down"))
	{
		// StateをWalkに変更する
		m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}
}