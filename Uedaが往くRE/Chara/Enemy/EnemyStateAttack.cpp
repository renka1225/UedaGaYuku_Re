#include "Game.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateAttack.h"

// 定数
namespace
{
    constexpr float kPunchEndTime = 30;
    constexpr float kKickEndTime = 120;
}

void EnemyStateAttack::Init(std::string attackName)
{
    m_attackKind = attackName;
    m_pEnemy->ChangeAnim(m_attackKind);

    if (m_attackKind == AnimName::kPunchStrong)
    {
        m_attackEndTime = kPunchEndTime;
    }
    else if (m_attackKind == AnimName::kKick)
    {
        m_attackEndTime = kKickEndTime;
    }
}

void EnemyStateAttack::Update(Stage& stage, Player& pPlayer)
{
    EnemyStateBase::Update(stage, pPlayer);
    m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

    // 攻撃終了した場合
    if (m_isAttackEnd)
    {
        // StateをIdleに変更する
        m_pEnemy->SetIsAttack(false);
        m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
        auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
        state->Init();
        return;
    }
    else
    {
        m_attackEndTime--;

        if (m_attackEndTime < 0)
        {
            m_isAttackEnd = true;
        }

        // 敵の攻撃とプレイヤーの当たり判定を取得
        if (m_attackKind == AnimName::kPunchStrong)
        {
            //bool isHitPunchCol = pPlayer.CheckHitPunchCol(*m_pEnemy, m_pEnemy->GetCol().armStartPos, m_pEnemy->GetCol().armEndPos, m_pEnemy->GetCol().armRadius);
            //if (isHitPunchCol)
            //{
            //    pPlayer.OnDamage(5);
            //}
        }
        else if (m_attackKind == AnimName::kKick)
        {
            //bool isHitKickCol = pPlayer.CheckHitKickCol(*m_pEnemy, m_pEnemy->GetCol().legStartPos, m_pEnemy->GetCol().legEndPos, m_pEnemy->GetCol().legRadius);
            //if (isHitKickCol)
            //{
            //    pPlayer.OnDamage(10);
            //}
        }
    }
}
