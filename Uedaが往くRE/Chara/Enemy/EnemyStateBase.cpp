#include "Game.h"
#include "Player.h"
#include "EnemyBase.h"
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
	constexpr float kMaxChaseRange = 1300.0f;	// プレイヤーを追いかける最大範囲
}

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy):
	m_upMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_leftMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_moveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_etoPVec(VGet(0.0f, 0.0f, 0.0f)),
	m_animEndTime(0.0f)
{
	m_pEnemy = pEnemy;
}

void EnemyStateBase::Update(Stage& stage, Player& pPlayer)
{
	if (m_pEnemy == nullptr) return;

	// ダウン状態中は状態を更新しない
	if (GetKind() == EnemyStateKind::kDeath) return;

	// HPが0以下になったら
	if (m_pEnemy->GetHp() <= 0.0f)
	{
		// StateをDeathに変更する
		m_nextState = std::make_shared<EnemyStateDeath>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDeath>(m_nextState);
		state->Init();
		return;
	}

	// 特定の状態中は更新しない
	bool isNotChange = m_pEnemy->GetIsAttack() || (GetKind() == EnemyStateKind::kAvoid) || (GetKind() == EnemyStateKind::kGuard);
	if (isNotChange) return;

	m_etoPVec = VSub(pPlayer.GetPos(), m_pEnemy->GetPos());
	float dist = VSize(m_etoPVec);	// 敵からプレイヤーまでの距離

	// プレイヤーを追いかける範囲内に入っている場合
	bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange;
	if (isChaseRange)
	{
		// すでに走り状態の場合は更新しない
		if (GetKind() == EnemyStateKind::kRun) return;

		// StateをRunに変更する
		m_nextState = std::make_shared<EnemyStateRun>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateRun>(m_nextState);
		state->Init();
		return;
	}
	else
	{
		// TODO:ランダムで攻撃をする
		int num = GetRand(100);
		if (num <= 20)
		{
			// StateをPunchに変更する
			m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
			state->Init(AnimName::kPunch);
			return;
		}
		else if (num <= 40)
		{
			// StateをKickに変更する
			m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
			state->Init(AnimName::kKick);
			return;
		}
		if (num <= 60)
		{
			// StateをAvoidに変更する
			m_nextState = std::make_shared<EnemyStateAvoid>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateAvoid>(m_nextState);
			state->Init();
			return;
		}
		if (num <= 80)
		{
			// StateをGuardに変更する
			m_nextState = std::make_shared<EnemyStateGuard>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateGuard>(m_nextState);
			state->Init();
			return;
		}

		//// 攻撃を受けた場合
		//if ()
		//{
		//	// StateをStateHitAttackに変更する
		//	m_nextState = std::make_shared<EnemyStateHitAttack>(m_pEnemy);
		//	auto state = std::dynamic_pointer_cast<EnemyStateHitAttack>(m_nextState);
		//	state->Init();
		//	return;
		//}
	}
}
