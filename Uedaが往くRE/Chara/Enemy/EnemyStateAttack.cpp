#include "Game.h"
#include "CharacterBase.h"
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

EnemyStateAttack::EnemyStateAttack(std::shared_ptr<EnemyBase> enemy):
    EnemyStateBase(enemy),
    m_attackEndTime(0.0f),
    m_isAttackEnd(false)
{
}

void EnemyStateAttack::Init(std::string attackName)
{
    m_attackKind = attackName;
    m_pEnemy->SetIsAttack(true);
    m_pEnemy->ChangeAnim(m_attackKind);
    m_attackEndTime = m_pEnemy->GetAnimTotalTime(m_attackKind);
}

void EnemyStateAttack::Update(Stage& stage, Player& pPlayer)
{
    EnemyStateBase::Update(stage, pPlayer);
    m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

    // 攻撃終了した場合
    if (m_isAttackEnd)
    {
        // StateをIdleに変更する
        pPlayer.SetIsInvincible(false);
        m_pEnemy->SetIsAttack(false);
        m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
        auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
        state->Init();
        return;
    }
    else
    {
        m_attackEndTime--;
        if (m_attackEndTime < 0.0f) m_isAttackEnd = true;

        // 特定の状態の場合はスキップする
        if (pPlayer.GetIsInvincible()) return;

        // 敵の攻撃とプレイヤーの当たり判定を取得
        if (m_attackKind == AnimName::kPunch)
        {
            bool isHitPunchCol = pPlayer.CheckHitPunchCol(m_pEnemy->GetCol(m_pEnemy->GetEnemyIndex()), 0);
            if (isHitPunchCol)
            {
                pPlayer.OnDamage(100);
                pPlayer.SetIsInvincible(true);
            }
        }
        else if (m_attackKind == AnimName::kKick)
        {
            bool isHitKickCol = pPlayer.CheckHitKickCol(m_pEnemy->GetCol(m_pEnemy->GetEnemyIndex()), 0);
            if (isHitKickCol)
            {
                pPlayer.OnDamage(300);
                pPlayer.SetIsInvincible(true);
            }
        }
    }
}
