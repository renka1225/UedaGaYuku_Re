#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateAttack.h"
#include "EnemyStateHitAttack.h"
#include "EnemyStateDeath.h"
#include "EnemyStateBase.h"

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy)
{
	m_pEnemy = pEnemy;
}

void EnemyStateBase::Update(Stage& stage, Player& pPlayer)
{
	if (m_pEnemy == nullptr) return;

	// 特定の状態中は状態を更新しない
	bool isNotChange = m_pEnemy->GetIsAttack() || std::dynamic_pointer_cast<EnemyStateDeath>(m_nextState);
	if (isNotChange) return;

	// HPが0以下になったら
	if (m_pEnemy->GetHp() <= 0)
	{
		// StateをDeathに変更する
		m_nextState = std::make_shared<EnemyStateDeath>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDeath>(m_nextState);
		state->Init();
		return;
	}

	// TODO:ランダムで攻撃をする
	int num = GetRand(100);
	if (num <= 7)
	{
		// StateをPunchに変更する
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(AnimName::kPunch);
		return;
	}
	else if (num <= 10)
	{
		// StateをKickに変更する
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(AnimName::kKick);
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
