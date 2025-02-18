#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateAvoid.h"

void EnemyStateAvoid::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kAvoid);
	m_animEndTime = m_pEnemy->GetAnimTotalTime(AnimName::kAvoid) / m_pEnemy->GetAnimSpeed();
}

void EnemyStateAvoid::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);

	m_animEndTime--;
	if (m_animEndTime < 0.0f)
	{
		ChangeStateIdle(pPlayer);
		return;
	}

	m_pEnemy->Move(m_moveVec, pStage, false);   // 移動情報を反映する
}