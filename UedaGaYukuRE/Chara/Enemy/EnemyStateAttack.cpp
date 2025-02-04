#include "Sound.h"
#include "CharacterBase.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateAttack.h"

namespace
{
    constexpr float kAttackEndAdjTime = 5.0f; // 攻撃終了の時間を調整
}

EnemyStateAttack::EnemyStateAttack(std::shared_ptr<EnemyBase> pEnemy):
    EnemyStateBase(pEnemy),
    m_attackEndTime(0.0f),
    m_isAttackEnd(false)
{
}

void EnemyStateAttack::Init()
{
    m_pEnemy->SetIsAttack(true);
    m_pEnemy->ChangeAnim(m_attackKind);
    m_attackEndTime = m_pEnemy->GetAnimTotalTime(m_attackKind);
}

void EnemyStateAttack::Update(Stage& pStage, Player& pPlayer)
{
    EnemyStateBase::Update(pStage, pPlayer);
    m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), pStage);   // 移動情報を反映する

    // 攻撃終了した場合
    if (m_isAttackEnd)
    {
        // StateをIdleに変更する
        pPlayer.SetIsInvincible(false);
        pPlayer.SetIsOnDamage(false);
        m_pEnemy->SetIsAttack(false);
        ChangeStateIdle(pPlayer);
        return;
    }
    else
    {
        m_attackEndTime--;
        if (m_attackEndTime <= kAttackEndAdjTime) m_isAttackEnd = true;

        // 特定の状態の場合はスキップする
        if (pPlayer.GetIsInvincible()) return;

        // 敵の攻撃とプレイヤーの当たり判定を取得
        if (m_attackKind == AnimName::kPunch)
        {
            bool isHitPunchCol = pPlayer.CheckHitPunchCol(m_pEnemy->GetCol(m_pEnemy->GetEnemyIndex()), 0);
            if (isHitPunchCol)
            {
                Sound::GetInstance().PlaySe(SoundName::kSe_attack);
                pPlayer.OnDamage(GetAttackPower(pPlayer));
                pPlayer.SetIsInvincible(true);
            }
        }
        else if (m_attackKind == AnimName::kKickRound)
        {
            bool isHitKickCol = pPlayer.CheckHitKickCol(m_pEnemy->GetCol(m_pEnemy->GetEnemyIndex()), 0);
            if (isHitKickCol)
            {
                Sound::GetInstance().PlaySe(SoundName::kSe_attack);
                pPlayer.OnDamage(GetAttackPower(pPlayer));
                pPlayer.SetIsInvincible(true);
            }
        }
    }
}

EnemyStateBase::EnemyStateKind EnemyStateAttack::GetKind()
{
    if (m_attackKind == AnimName::kPunch)
    {
        return EnemyStateKind::kPunch;
    }
    else if (m_attackKind == AnimName::kKickRound)
    {
        return EnemyStateKind::kKick;
    }

    return EnemyStateKind::kIdle;
}

#ifdef _DEBUG
std::string EnemyStateAttack::GetStateName()
{
    if (m_attackKind == AnimName::kPunch)  return "パンチ中";
    else return "キック中";
}
#endif

float EnemyStateAttack::GetAttackPower(Player& pPlayer)
{
    // ステータス取得
    auto status = m_pEnemy->GetStatus();

    if (pPlayer.GetIsGuard()) return 0.0f; // プレイヤーがガード中はダメージが入らないようにする
    if (m_attackKind == AnimName::kPunch) return status.atkPowerPunch1;
    if (m_attackKind == AnimName::kKickRound)  return status.atkPowerKick;

    return 0.0f;
}
