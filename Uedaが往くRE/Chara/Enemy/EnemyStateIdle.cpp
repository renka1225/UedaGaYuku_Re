#include "EnemyBase.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"
#include "EnemyStateIdle.h"

/// <summary>
/// 初期化
/// </summary>
void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim("StandIdle");
}

/// <summary>
/// 更新
/// </summary>
void EnemyStateIdle::Update(Stage& stage)
{
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

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
