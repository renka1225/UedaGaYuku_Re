#include "Game.h"
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

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num > 30)
	{
		// StateをWalkに変更する
		std::shared_ptr<EnemyStateIdle> state = std::make_shared<EnemyStateIdle>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
#endif
}
