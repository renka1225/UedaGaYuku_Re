#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHitAttack.h"

void EnemyStateHitAttack::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kDamage);
}

void EnemyStateHitAttack::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage, false);   // 移動情報を反映する

	m_animEndTime--;

	// 無敵状態を解除する
	if (m_animEndTime <= 0.0f)
	{
		m_pEnemy->SetIsOnDamage(false);
		m_pEnemy->SetIsInvincible(false);
	}

	if (m_animEndTime < 0.0f)
	{
		// StateをIdleに変更する
		ChangeStateIdle(pPlayer);
		return;
	}
}

void EnemyStateHitAttack::SetInvincibleTime(Player& pPlayer)
{
	// プレイヤーの攻撃アニメーションによって無敵時間を変える
	m_animEndTime = pPlayer.GetAnimTotalTime(pPlayer.GetCurrentAnim());
}
