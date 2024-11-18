#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHitAttack.h"

void EnemyStateHitAttack::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kDamage);
	m_animEndTime = m_pEnemy->GetAnimTotalTime(AnimName::kDamage);
}

void EnemyStateHitAttack::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage, false);   // 移動情報を反映する

	m_animEndTime--;
	if (m_animEndTime < 0.0f)
	{
		// StateをIdleに変更する
		m_pEnemy->SetIsOnDamage(false);
		std::shared_ptr<EnemyStateIdle> state = std::make_shared<EnemyStateIdle>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
	
}
