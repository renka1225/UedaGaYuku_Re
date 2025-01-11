#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHitAttack.h"

namespace
{
	constexpr float kDamageReleaseTime = 8; // 無敵状態を解除する時間
}

void EnemyStateHitAttack::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kDamage);
	m_animEndTime = m_pEnemy->GetAnimTotalTime(AnimName::kDamage);
}

void EnemyStateHitAttack::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(m_moveVec, pStage, false);   // 移動情報を反映する

	m_animEndTime--;

	// 硬直フレーム時に無敵状態を解除する
	float totalTime = m_pEnemy->GetAnimTotalTime(AnimName::kDamage);
	if (m_animEndTime >= totalTime - kDamageReleaseTime)
	{
		m_pEnemy->SetIsOnDamage(false);
	}

	if (m_animEndTime < 0.0f)
	{
		// StateをIdleに変更する
		ChangeStateIdle(pPlayer);
		return;
	}
	
}
