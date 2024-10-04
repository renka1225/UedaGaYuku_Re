#include "EnemyBase.h"
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
	//EnemyStateBase::Update(stage);
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する
}
