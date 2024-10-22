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

	VECTOR upMoveVec;	 // 上ボタンを入力をしたときの移動方向ベクトル
	VECTOR leftMoveVec;	 // 左ボタンを入力をしたときの移動方向ベクトル
	VECTOR moveVec;		 // 移動ベクトル
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num == 1)
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<EnemyStateRun>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateRun>(m_nextState);
		state->Init();
		return;
	}
	else if (num == 10)
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
		state->Init();
		return;
	}
#endif
}
