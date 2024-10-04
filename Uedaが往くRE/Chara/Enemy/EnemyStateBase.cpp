#include "EnemyStateBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pEnemy"></param>
EnemyStateBase::EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy)
{
	m_pEnemy = pEnemy;
}