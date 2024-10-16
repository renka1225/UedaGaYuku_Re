#include "EnemyBase.h"
#include "EnemyStateHitAttack.h"
#include "EnemyStateDeath.h"
#include "EnemyStateBase.h"

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy)
{
	m_pEnemy = pEnemy;
}

void EnemyStateBase::Update(Stage& stage)
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

	// 攻撃を受けた場合
	//if (m_pEnemy->OnDamage())
	//{
	//	// StateをStateHitAttackに変更する
	//	m_nextState = std::make_shared<EnemyStateHitAttack>(m_pEnemy);
	//	auto state = std::dynamic_pointer_cast<EnemyStateHitAttack>(m_nextState);
	//	state->Init();
	//	return;
	//}
}
