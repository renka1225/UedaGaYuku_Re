#include "Input.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"

void PlayerStateGuard::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kGuard);
	m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kGuard);
}

void PlayerStateGuard::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
	m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する

	// ボタンを離した場合
	if (input.IsReleased(InputId::kGuard))
	{
		// StateをIdleに変更する
		m_pPlayer->SetIsGuard(false);
		std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
		m_nextState = state;
		state->Init();
		return;
	}
}
