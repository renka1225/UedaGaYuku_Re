#include "CharacterBase.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateMove.h"

void EnemyStateMove::Init()
{
    m_pEnemy->ChangeAnim(m_animKind);
}

void EnemyStateMove::Update(Stage& pStage, Player& pPlayer)
{
    EnemyStateBase::Update(pStage, pPlayer);

    // 移動速度を決める
    float moveSpeed = 0.0f;
    if(m_animKind == AnimName::kWalk)
    {
        moveSpeed = m_pEnemy->GetStatus().walkSpeed;
    }
    else if (m_animKind == AnimName::kRun)
    {
        moveSpeed = m_pEnemy->GetStatus().runSpeed;
    }

    // プレイヤーに向かって移動する
    m_moveVec = VScale(VNorm(m_pEnemy->GetEToPVec()), moveSpeed);
    m_pEnemy->Move(m_moveVec, pStage);
}

EnemyStateBase::EnemyStateKind EnemyStateMove::GetKind()
{
    if (m_animKind == AnimName::kWalk)
    {
        return EnemyStateKind::kWalk;
    }
    else if (m_animKind == AnimName::kRun)
    {
        return EnemyStateKind::kRun;
    }

    return EnemyStateKind::kIdle;
}

std::string EnemyStateMove::GetStateName()
{
    if (m_animKind == AnimName::kWalk)  return "歩き";
    if(m_animKind == AnimName::kRun) return "走り";
}
