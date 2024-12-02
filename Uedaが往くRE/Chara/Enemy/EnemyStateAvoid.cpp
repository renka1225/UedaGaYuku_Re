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
		// StateをIdleに変更する
		std::shared_ptr<EnemyStateIdle> state = std::make_shared<EnemyStateIdle>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}

	// 背後に移動する
	m_moveVec = VScale(m_pEnemy->GetDir(), -1.0f);
	m_moveVec = VScale(VNorm(m_moveVec), m_pEnemy->GetStatus().avoidDist);
	m_moveVec.y = 0.0f;
	m_pEnemy->Move(m_moveVec, pStage, false);   // 移動情報を反映する
}