#include "Game.h"
#include "Input.h"
#include "Weapon.h"
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

PlayerStateAttack::PlayerStateAttack(std::shared_ptr<Player> player):
    PlayerStateBase(player),
    m_isAttackEnd(false)
{
}

void PlayerStateAttack::Init(std::string attackName)
{
    m_attackKind = attackName;
	m_pPlayer->ChangeAnim(m_attackKind);

    if (m_attackKind == AnimName::kPunch1)
    {
        m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kPunch1);
    }
    if (m_attackKind == AnimName::kPunch2)
    {
        m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kPunch2);
    }
    if (m_attackKind == AnimName::kPunch3)
    {
        m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kPunch3);
    }
    else if (m_attackKind == AnimName::kKick)
    {
        m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kKick);
    }
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

    // パンチコマンドが入力されている場合
    if (m_pPlayer->CheckCommand({ InputId::kPunch, InputId::kPunch }, m_pPlayer->GetInputLog()))
    {
        // 2コンボ目に移行する
        if (m_attackKind == AnimName::kPunch1)
        {
            printfDx("2コンボ目\n");
            Init(AnimName::kPunch2);
            return;
        }
        // 3コンボ目に移行する
        else if (m_attackKind == AnimName::kPunch2)
        {
            printfDx("3コンボ目\n");
            Init(AnimName::kPunch3);
            return;
        }
        // 現在最終コンボの場合
        else if (m_attackKind == AnimName::kPunch3)
        {
            // StateをIdleに変更する
            ChangeStateIdle();
            return;
        }
    }

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        weapon.SetIsHitAttack(false);
        for (auto& enemy : pEnemy)
        {
            if (enemy == nullptr) continue;
            enemy->SetIsInvincible(false);
        }

        // StateをIdleに変更する
        m_pPlayer->SetIsAttack(false);
        ChangeStateIdle();
    }
    else
    {
        UpdateAttack(weapon, pEnemy);
    }
}

void PlayerStateAttack::UpdateAttack(Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    m_animEndTime--;
    if (m_animEndTime < 0.0f) m_isAttackEnd = true;

    for (auto& enemy : pEnemy)
    {
        // 特定の状態の場合はスキップする
        bool isSkip = enemy == nullptr || enemy->GetIsInvincible();
        if (isSkip) continue;

        // パンチ状態かチェック
        bool isPunch = m_attackKind == AnimName::kPunch1 || m_attackKind == AnimName::kPunch2 || m_attackKind == AnimName::kPunch3;

        // 武器掴み中の場合
        if (m_pPlayer->GetIsGrabWeapon())
        {
            // 武器と敵の当たり判定を取得
            bool isHitWeaponCol = weapon.CheckWeaopnCol(enemy->GetCol(enemy->GetEnemyIndex()), *m_pPlayer);
            if (isHitWeaponCol)
            {
                // TODO:片手武器、両手武器によって攻撃力変える
                enemy->OnDamage(m_pPlayer->GetStatus().atkPowerOneHandWeapon);
                enemy->SetIsInvincible(true);
                weapon.DecrementDurability();
            }
        }
        // パンチ攻撃
        else if (isPunch)
        {
            if (enemy == nullptr) continue;

            // パンチ攻撃と敵の当たり判定を取得
            bool isHitPunchCol = enemy->CheckHitPunchCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitPunchCol)
            {
                enemy->OnDamage(m_pPlayer->GetStatus().atkPowerPunch1);
                enemy->SetIsInvincible(true);
            }
        }
        // キック攻撃
        else if (m_attackKind == AnimName::kKick)
        {
            if (enemy == nullptr) continue;

            // キック攻撃と敵の当たり判定を取得
            bool isHitKickCol = enemy->CheckHitKickCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitKickCol)
            {
                enemy->OnDamage(m_pPlayer->GetStatus().atkPowerKick);
                enemy->SetIsInvincible(true);
            }
        }
    }
}

#ifdef _DEBUG
std::string PlayerStateAttack::GetStateName()
{
    if (m_attackKind == AnimName::kPunch1) return "パンチ1中";
    else if (m_attackKind == AnimName::kPunch1) return "パンチ2中";
    if (m_attackKind == AnimName::kPunch1) return "パンチ3中";
    else if (m_attackKind == AnimName::kKick)  return "キック中";

    return "アニメーションなし";
}
#endif
