#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateGuard.h"

void EnemyStateGuard::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kGuard);
}

void EnemyStateGuard::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage); // 移動情報を反映する
}
