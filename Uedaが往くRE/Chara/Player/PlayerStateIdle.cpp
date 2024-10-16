#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Player.h"
#include "PlayerStateAttack.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim("StandIdle");
}

void PlayerStateIdle::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, pEnemy);
	m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する
	
	// 移動ボタンが押されている場合
	if (input.IsPressing(InputId::kA) && (m_analogX != 0 || m_analogY != 0))
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
		state->Init();
		return;
	}
	else if (m_analogX != 0 || m_analogY != 0)
	{
		// StateをWalkに変更する
		m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}
}