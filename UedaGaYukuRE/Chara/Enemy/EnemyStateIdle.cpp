#include "EnemyBase.h"
#include "EnemyStateDeath.h"
#include "EnemyStateIdle.h"

void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim(m_animKind);
}

void EnemyStateIdle::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage); // 移動情報を反映する
}

#ifdef _DEBUG
std::string EnemyStateIdle::GetStateName()
{
	if (m_animKind == AnimName::kIdleStand)  return "待機";
	if (m_animKind == AnimName::kIdleFight)  return "バトル中_待機";
	return "なし";
}
#endif