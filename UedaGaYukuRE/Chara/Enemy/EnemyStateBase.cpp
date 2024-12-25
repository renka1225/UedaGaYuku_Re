#include "EffectManager.h"
#include "Sound.h"
#include "Player.h"
#include "EnemyAI.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateRun.h"
#include "EnemyStateWalk.h"
#include "EnemyStateAvoid.h"
#include "EnemyStateGuard.h"
#include "EnemyStateAttack.h"
#include "EnemyStateHitAttack.h"
#include "EnemyStateDeath.h"
#include "EnemyStateBase.h"

namespace
{
	constexpr float kMinChaseRange = 200.0f; // プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 800.0f; // プレイヤーを追いかける最大範囲

	constexpr float kMinHp = 0.0f;	// 最小HP
}

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy):
	m_pEnemy(pEnemy),
	m_upMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_leftMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_moveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_animEndTime(0.0f)
{
}

void EnemyStateBase::Update(Stage& pStage, Player& pPlayer)
{
	if (m_pEnemy == nullptr) return;

	// ダウン状態中は状態を更新しない
	if (GetKind() == EnemyStateKind::kDeath) return;

	// 動けない状態の場合
	if (!m_pEnemy->GetIsPossibleMove())
	{
		ChangeStateIdle();
		return;
	}

	// HPが0以下になった場合
	if (m_pEnemy->GetHp() <= kMinHp)
	{
		ChangeStateDeath();
		return;
	}

	// 攻撃を受けた場合
	if (m_pEnemy->GetIsOnDamage())
	{	
		ChangeStateDamage();
		return;
	}

	EnemyStateKind nextState = m_pEnemy->GetEnemyAI()->GetNextState();
	ChangeState(nextState);

	//float dist = VSize(m_pEnemy->GetEToPVec());	// 敵からプレイヤーまでの距離
	//
	//// プレイヤーを追いかける範囲内に入っている場合
	//bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange;
	//if (isChaseRange)
	//{
	//	// 特定の状態中は更新しない
	//	bool isNotChange = m_pEnemy->GetIsAttack() || (GetKind() == EnemyStateKind::kAvoid) || (GetKind() == EnemyStateKind::kGuard) || 
	//		(GetKind() == EnemyStateKind::kDamage) || (GetKind() == EnemyStateKind::kRun);
	//	if (isNotChange) return;

	//	// StateをRunに変更する
	//	ChangeStateRun();
	//	return;
	//}
	//// プレイヤーから離れた場合
	//else if (dist >= kMaxChaseRange)
	//{
	//	// バトルを終了状態にする
	//	pPlayer.SetIsBattle(false);

	//	// 待機か歩きのみを行う
	//	ChangeStateIdle();
	//	return;
	//}
	//else
	//{
	//	// 特定の状態中は更新しない
	//	bool isNotChange = !pPlayer.GetIsBattle() || m_pEnemy->GetIsAttack() || 
	//		(GetKind() == EnemyStateKind::kAvoid) || (GetKind() == EnemyStateKind::kGuard) || (GetKind() == EnemyStateKind::kDamage);
	//	if (isNotChange) return;
	//}
}

void EnemyStateBase::ChangeState(EnemyStateKind nextState)
{
	if (GetKind() == nextState) return;

	switch (nextState)
	{
	case EnemyStateBase::EnemyStateKind::kWalk:
		//printfDx("歩き\n");
		ChangeStateWalk();
		break;
	case EnemyStateBase::EnemyStateKind::kRun:
		//printfDx("走り\n");
		ChangeStateRun();
		break;
	case EnemyStateBase::EnemyStateKind::kAvoid:
		//printfDx("回避\n");
		ChangeStateAvoid();
		break;
	case EnemyStateBase::EnemyStateKind::kPunch:
		//printfDx("パンチ\n");
		ChangeStatePunch();
		break;
	case EnemyStateBase::EnemyStateKind::kKick:
		//printfDx("キック\n");
		ChangeStateKick();
		break;
	default:
		break;
	}
}

void EnemyStateBase::ChangeStateIdle()
{
	if (GetKind() == EnemyStateKind::kIdle) return;

	std::shared_ptr<EnemyStateIdle> state = std::make_shared<EnemyStateIdle>(m_pEnemy);
	m_nextState = state;
	state->Init();
}

void EnemyStateBase::ChangeStateWalk()
{
	std::shared_ptr<EnemyStateWalk> state = std::make_shared<EnemyStateWalk>(m_pEnemy);
	m_nextState = state;
	state->Init();
}

void EnemyStateBase::ChangeStateRun()
{
	std::shared_ptr<EnemyStateRun> state = std::make_shared<EnemyStateRun>(m_pEnemy);
	m_nextState = state;
	state->Init();
}

void EnemyStateBase::ChangeStatePunch()
{
	if (GetKind() == EnemyStateKind::kPunch) return;

	printfDx("パンチ攻撃\n");
	std::shared_ptr<EnemyStateAttack> state = std::make_shared<EnemyStateAttack>(m_pEnemy);
	m_nextState = state;
	state->SetAttackKind(AnimName::kPunchStrong);
	state->Init();
}

void EnemyStateBase::ChangeStateKick()
{
	std::shared_ptr<EnemyStateAttack> state = std::make_shared<EnemyStateAttack>(m_pEnemy);
	m_nextState = state;
	state->SetAttackKind(AnimName::kKick);
	state->Init();
}

void EnemyStateBase::ChangeStateGuard()
{
	std::shared_ptr<EnemyStateGuard> state = std::make_shared<EnemyStateGuard>(m_pEnemy);
	m_nextState = state;
	state->Init();
}

void EnemyStateBase::ChangeStateAvoid()
{
	Sound::GetInstance().PlaySe(SoundName::kSe_avoid);
	std::shared_ptr<EnemyStateAvoid> state = std::make_shared<EnemyStateAvoid>(m_pEnemy);
	m_nextState = state;
	state->Init();
}

void EnemyStateBase::ChangeStateDamage()
{
	if (GetKind() == EnemyStateKind::kDamage) return;

	std::shared_ptr<EnemyStateHitAttack> state = std::make_shared<EnemyStateHitAttack>(m_pEnemy);
	m_nextState = state;
	state->Init();

	// ダメージエフェクトを表示
	EffectManager::GetInstance().Add("attack", m_pEnemy->GetPos());
}

void EnemyStateBase::ChangeStateDeath()
{
	std::shared_ptr<EnemyStateDeath> state = std::make_shared<EnemyStateDeath>(m_pEnemy);
	m_nextState = state;
	state->Init();
}