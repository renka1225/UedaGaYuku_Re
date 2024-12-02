#include "EnemyBase.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"
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

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num == 5)
	{
		// StateをWalkに変更する
		std::shared_ptr<EnemyStateWalk> state = std::make_shared<EnemyStateWalk>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
#endif
}