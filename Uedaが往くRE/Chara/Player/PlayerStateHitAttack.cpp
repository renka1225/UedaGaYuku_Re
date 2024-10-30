#include "Game.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kDamage);
	m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kDamage);
}

void PlayerStateHitAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
	m_pPlayer->Move(m_moveVec, stage, false);   // 移動情報を反映する

	m_animEndTime--;
	if (m_animEndTime < 0.0f)
	{
		// StateをIdleに変更する
		m_pPlayer->SetIsOnDamage(false);
		std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
		m_nextState = state;
		state->Init();
		return;
	}
}