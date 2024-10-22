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

	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage); // 移動情報を反映する

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num == 1)
	{
		// StateをWalkに変更する
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init();
		return;
	}
	else if (num == 10)
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<EnemyStateRun>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateRun>(m_nextState);
		state->Init();
		return;
	}
#endif
}