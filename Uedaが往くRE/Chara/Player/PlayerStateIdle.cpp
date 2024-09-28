#include "Input.h"
#include "Player.h"
#include "PlayerStateIdle.h"

/// <summary>
/// 初期化
/// </summary>
void PlayerStateIdle::Init()
{
	//m_pPlayer->ChangeAnim("Idle");
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力処理</param>
void PlayerStateIdle::Update(Input input)
{
	// 移動ボタンが押されている場合
	if (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down"))
	{
		// StateをMoveに変更する
		//m_nextState = std::make_shared<PlayStateWalk>(m_pPlayer);
	}
}
