#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateRun.h"
#include "EnemyStateWalk.h"

void EnemyStateWalk::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kWalk);
}

void EnemyStateWalk::Update(Stage& stage, Player& pPlayer)
{
	EnemyStateBase::Update(stage, pPlayer);

	m_moveVec = VScale(VNorm(m_etoPVec), m_pEnemy->GetStatus().walkSpeed); // プレイヤーに向かって歩く
	m_pEnemy->Move(m_moveVec, stage); // 移動情報を反映する

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num <= 10)
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
		state->Init();
		return;
	}
#endif
}
