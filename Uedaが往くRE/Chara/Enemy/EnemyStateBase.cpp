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
	if (num <= 5)
	{
		// StateをWalkに変更する
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(AnimName::kPunchStrong);
		return;
	}
	else if (num <= 10)
	{
		// StateをRunに変更する
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(AnimName::kKick);
		return;
	}

	// 攻撃を受けた場合
	//if ()
	//{
	//	// StateをStateHitAttackに変更する
	//	m_nextState = std::make_shared<EnemyStateHitAttack>(m_pEnemy);
	//	auto state = std::dynamic_pointer_cast<EnemyStateHitAttack>(m_nextState);
	//	state->Init();
	//	return;
	//}
}
