#include "Game.h"
#include "EffectManager.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateRun.h"
#include "EnemyStateAvoid.h"
#include "EnemyStateGuard.h"
#include "EnemyStateAttack.h"
#include "EnemyStateHitAttack.h"
#include "EnemyStateDeath.h"
#include "EnemyStateBase.h"

namespace
{
	constexpr float kMinChaseRange = 150.0f;	// プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 600.0f;	// プレイヤーを追いかける最大範囲
}

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy):
	m_upMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_leftMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_moveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_animEndTime(0.0f)
{
	m_pEnemy = pEnemy;
}

void EnemyStateBase::Update(Stage& pStage, Player& pPlayer)
{
	if (m_pEnemy == nullptr) return;

	// ダウン状態中は状態を更新しない
	if (GetKind() == EnemyStateKind::kDeath) return;

	// 動けない状態の場合
	if (!m_pEnemy->GetIsPossibleMove())
	{
		if (GetKind() == EnemyStateKind::kIdle) return;

		// StateをIdleに変更する
		std::shared_ptr<EnemyStateIdle> state = std::make_shared<EnemyStateIdle>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}

	// 攻撃を受けた場合
	if (m_pEnemy->GetIsOnDamage() && !(GetKind() == EnemyStateKind::kDamage))
	{	
		// StateをStateHitAttackに変更する
		std::shared_ptr<EnemyStateHitAttack> state = std::make_shared<EnemyStateHitAttack>(m_pEnemy);
		m_nextState = state;
		state->Init();

		// ダメージエフェクトを表示
		EffectManager::GetInstance().Add("attack", m_pEnemy->GetPos());
		return;
	}

	// HPが0以下になったら
	if (m_pEnemy->GetHp() <= 0.0f)
	{
		// StateをDeathに変更する
		std::shared_ptr<EnemyStateDeath> state = std::make_shared<EnemyStateDeath>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}

	float dist = VSize(m_pEnemy->GetEToPVec());	// 敵からプレイヤーまでの距離

	// プレイヤーを追いかける範囲内に入っている場合
	bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange;
	if (isChaseRange)
	{
		// 特定の状態中は更新しない
		bool isNotChange = m_pEnemy->GetIsAttack() || (GetKind() == EnemyStateKind::kAvoid) || (GetKind() == EnemyStateKind::kGuard) || 
			(GetKind() == EnemyStateKind::kDamage) || (GetKind() == EnemyStateKind::kRun);
		if (isNotChange) return;

		// StateをRunに変更する
		std::shared_ptr<EnemyStateRun> state = std::make_shared<EnemyStateRun>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
	// プレイヤーから離れた場合
	else if (dist >= kMaxChaseRange)
	{
		// バトルを終了状態にする
		pPlayer.SetIsBattle(false);

		// 待機か歩きのみを行う
	}
	else
	{
		// 特定の状態中は更新しない
		bool isNotChange = !pPlayer.GetIsBattle() || m_pEnemy->GetIsAttack() || 
			(GetKind() == EnemyStateKind::kAvoid) || (GetKind() == EnemyStateKind::kGuard) || (GetKind() == EnemyStateKind::kDamage);
		if (isNotChange) return;

		// TODO:ランダムで攻撃をする
		AttackRand();
	}
}

void EnemyStateBase::AttackRand()
{
	int num = GetRand(100);
	if (num <= 10)
	{
		// StateをPunchに変更する
		std::shared_ptr<EnemyStateAttack> state = std::make_shared<EnemyStateAttack>(m_pEnemy);
		m_nextState = state;
		state->Init(AnimName::kPunchStrong);
		return;
	}
	else if (num <= 20)
	{
		// StateをKickに変更する
		std::shared_ptr<EnemyStateAttack> state = std::make_shared<EnemyStateAttack>(m_pEnemy);
		m_nextState = state;
		state->Init(AnimName::kKick);
		return;
	}
	if (num <= 30)
	{
		// StateをAvoidに変更する
		std::shared_ptr<EnemyStateAvoid> state = std::make_shared<EnemyStateAvoid>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
	if (num <= 40)
	{
		// StateをGuardに変更する
		std::shared_ptr<EnemyStateGuard> state = std::make_shared<EnemyStateGuard>(m_pEnemy);
		m_nextState = state;
		state->Init();
		return;
	}
}
