#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"
#include "EnemyStateDeath.h"
#include "EnemyStateIdle.h"

void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kIdleStand);
}

void EnemyStateIdle::Update(Stage& stage, Player& pPlayer)
{
	EnemyStateBase::Update(stage, pPlayer);
	m_pEnemy->Move(m_moveVec, stage); // 移動情報を反映する

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
	//else if (num == 10)
	//{
	//	// StateをRunに変更する
	//	std::shared_ptr<EnemyStateRun> state = std::make_shared<EnemyStateRun>(m_pEnemy);
	//	m_nextState = state;
	//	state->Init();
	//	return;
	//}
#endif
}