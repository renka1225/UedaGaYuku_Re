#include "EnemyStateBase.h"

EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy)
{
	m_pEnemy = pEnemy;
}