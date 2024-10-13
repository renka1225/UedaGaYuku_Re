#include "EnemyBase.h"
#include "EnemyStateAttack.h"

void EnemyStateAttack::Init()
{
	m_pEnemy->ChangeAnim("Attack");
}

void EnemyStateAttack::Update(Stage& stage)
{
}
