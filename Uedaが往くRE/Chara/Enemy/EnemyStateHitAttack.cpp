#include "EnemyBase.h"
#include "EnemyStateHitAttack.h"

void EnemyStateHitAttack::Init()
{
	m_pEnemy->ChangeAnim("Damage");
}

void EnemyStateHitAttack::Update(Stage& stage)
{
	EnemyStateBase::Update(stage);
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// TODO:アニメーションが終わったら待機状態に戻る
	
}
