#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"

void EnemyStateRun::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kRun);
}

void EnemyStateRun::Update(Stage& stage, Player& pPlayer)
{
	EnemyStateBase::Update(stage, pPlayer);

    m_moveVec = VScale(VNorm(m_pEnemy->GetEToPVec()), m_pEnemy->GetStatus().runSpeed); // プレイヤーを追従する
    m_pEnemy->Move(m_moveVec, stage); // 移動情報を反映する
}
