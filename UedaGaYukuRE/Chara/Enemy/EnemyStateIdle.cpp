#include "EnemyBase.h"
#include "EnemyStateDeath.h"
#include "EnemyStateIdle.h"

void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kIdleStand);
}

void EnemyStateIdle::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage); // 移動情報を反映する
}