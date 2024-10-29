#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"

void EnemyStateRun::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kRun);
}

void EnemyStateRun::Update(Stage& stage, Player& pPlayer)
{
	EnemyStateBase::Update(stage, pPlayer);
	m_pEnemy->Move(m_moveVec, stage); // 移動情報を反映する

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num == 1)
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init();
		return;
	}
	else if(num == 10)
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
		state->Init();
		return;
	}
#endif
}
