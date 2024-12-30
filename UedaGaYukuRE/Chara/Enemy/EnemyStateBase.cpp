#include "EffectManager.h"
#include "Sound.h"
#include "Player.h"
#include "EnemyAI.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateMove.h"
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
}

void EnemyStateBase::ChangeState(EnemyStateKind nextState)
{
	if (GetKind() == nextState) return;

	switch (nextState)
	{
	case EnemyStateBase::EnemyStateKind::kWalk:
		ChangeStateWalk();
		break;
	case EnemyStateBase::EnemyStateKind::kRun:
		ChangeStateRun();
		break;
	case EnemyStateBase::EnemyStateKind::kAvoid:
		ChangeStateAvoid();
		break;
	case EnemyStateBase::EnemyStateKind::kPunch:
		ChangeStatePunch();
		break;
	case EnemyStateBase::EnemyStateKind::kKick:
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
	std::shared_ptr<EnemyStateMove> state = std::make_shared<EnemyStateMove>(m_pEnemy);
	m_nextState = state;
	state->SetAnimKind(AnimName::kWalk);
	state->Init();
}

void EnemyStateBase::ChangeStateRun()
{
	std::shared_ptr<EnemyStateMove> state = std::make_shared<EnemyStateMove>(m_pEnemy);
	m_nextState = state;
	state->SetAnimKind(AnimName::kRun);
	state->Init();
}

void EnemyStateBase::ChangeStatePunch()
{
	std::shared_ptr<EnemyStateAttack> state = std::make_shared<EnemyStateAttack>(m_pEnemy);
	m_nextState = state;
	state->SetAttackKind(AnimName::kPunch);
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
	EffectManager::GetInstance().Add(EffectName::kAttack, m_pEnemy->GetPos());
}

void EnemyStateBase::ChangeStateDeath()
{
	std::shared_ptr<EnemyStateDeath> state = std::make_shared<EnemyStateDeath>(m_pEnemy);
	m_nextState = state;
	state->Init();
}