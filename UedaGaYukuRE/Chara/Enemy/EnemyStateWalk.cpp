#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateRun.h"
#include "EnemyStateWalk.h"

void EnemyStateWalk::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kWalk);
}

void EnemyStateWalk::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);

	m_moveVec = VScale(VNorm(m_pEnemy->GetEToPVec()), m_pEnemy->GetStatus().walkSpeed); // プレイヤーに向かって歩く
	m_pEnemy->Move(m_moveVec, pStage); // 移動情報を反映する

#ifdef _DEBUG
	// 仮実装
	int num = GetRand(100);
	if (num <= 10)
	{
		ChangeStateIdle();
		return;
	}
#endif
}
