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

    // 現在のアニメーション終了時間を取得する
    m_animEndTime = GetAnimEndTime();
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        weapon.SetIsHitAttack(false);
        for (auto& enemy : pEnemy)
        {
            if (enemy == nullptr) continue;
            // 敵の無敵状態を解除する
            enemy->SetIsInvincible(false);
        }

        // StateをIdleに変更する
        ChangeStateIdle();
        m_pPlayer->SetIsAttack(false);

        printfDx("攻撃終了\n");
    }
    else
    {
        UpdateAttack(weapon, pEnemy);
    }
}

void PlayerStateAttack::UpdateAttack(Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    for (auto& enemy : pEnemy)
    {
        // 特定の状態の場合はスキップする
        bool isSkip = (enemy == nullptr) || (enemy->GetIsInvincible()) ||  (m_attackKind == AnimName::kKickHeat);
        if (isSkip) continue;

        // パンチ状態かチェック
        bool isPunch = m_attackKind == AnimName::kPunch1 || m_attackKind == AnimName::kPunch2 || m_attackKind == AnimName::kPunch3;
        // パンチ攻撃
        if (isPunch)
        {
            // パンチ攻撃と敵の当たり判定を取得
            bool isHitPunchCol = enemy->CheckHitPunchCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitPunchCol)
            {
                enemy->OnDamage(GetAttackPower());
                // 敵を無敵状態にする
                enemy->SetIsInvincible(true);
            }
        }
        // キック攻撃
        else if (m_attackKind == AnimName::kKick)
        {
            // キック攻撃と敵の当たり判定を取得
            bool isHitKickCol = enemy->CheckHitKickCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitKickCol)
            {
                enemy->OnDamage(GetAttackPower());
                // 敵を無敵状態にする
                enemy->SetIsInvincible(true);
            }
        }
        // 武器掴み中の場合
        else if (m_pPlayer->GetIsGrabWeapon())
        {
            // 武器と敵の当たり判定を取得
            bool isHitWeaponCol = weapon.CheckWeaopnCol(enemy->GetCol(enemy->GetEnemyIndex()), *m_pPlayer);
            if (isHitWeaponCol)
            {
                // 片手武器、両手武器によって攻撃力変える
                enemy->OnDamage(GetAttackPower());
                weapon.DecrementDurability();

                // 敵を無敵状態にする
                enemy->SetIsInvincible(true);
            }
        }
        // 必殺技発動中
        else if (m_attackKind == AnimName::kSpecialAttack)
        {
            bool isHitKickCol = enemy->CheckHitKickCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitKickCol)
            {
                enemy->OnDamage(GetAttackPower());
            }
        }
    }

    // アニメーション時間の更新
    m_animEndTime--;
    if (m_animEndTime > 0.0f) return;

    // パンチコマンドが入力されている場合
    if (m_pPlayer->CheckCommand({ InputId::kPunch, InputId::kPunch }, m_pPlayer->GetInputLog()))
    {
        // 2コンボ目に移行する
        if (m_attackKind == AnimName::kPunch1)
        {
            Init(AnimName::kPunch2);
            return;
        }
        // 3コンボ目に移行する
        else if (m_attackKind == AnimName::kPunch2)
        {
            Init(AnimName::kPunch3);
            return;
        }
        // 現在最終コンボの場合
        else if (m_attackKind == AnimName::kPunch3)
        {
            // 攻撃を終了する
            m_isAttackEnd = true;
            return;
        }
    }
    // コンボ入力がない場合
    else 
    {
        // 攻撃を終了する
        m_isAttackEnd = true;
    }
}

float PlayerStateAttack::GetAnimEndTime()
{
    if (m_attackKind == AnimName::kPunch1) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch1);
    if (m_attackKind == AnimName::kPunch2) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch2);
    if (m_attackKind == AnimName::kPunch3) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch3);
    if (m_attackKind == AnimName::kKick) return m_pPlayer->GetAnimTotalTime(AnimName::kKick);
    if (m_attackKind == AnimName::kKickHeat) return m_pPlayer->GetAnimTotalTime(AnimName::kKickHeat);
}

float PlayerStateAttack::GetAttackPower()
{
    // ステータス取得
    auto status =  m_pPlayer->GetStatus();

    if (m_attackKind == AnimName::kPunch1) return status.atkPowerPunch1;
    if (m_attackKind == AnimName::kPunch2) return status.atkPowerPunch2;
    if (m_attackKind == AnimName::kPunch3) return status.atkPowerPunch3;
    if (m_attackKind == AnimName::kKick)  return status.atkPowerKick;
    if (m_attackKind == AnimName::kKickHeat)  return status.atkPowerKick * 1.5f;
}

#ifdef _DEBUG
std::string PlayerStateAttack::GetStateName()
{
    if (m_attackKind == AnimName::kPunch1) return "パンチ1中";
    if (m_attackKind == AnimName::kPunch2) return "パンチ2中";
    if (m_attackKind == AnimName::kPunch3) return "パンチ3中";
    if (m_attackKind == AnimName::kKick)  return "キック中";
    if (m_attackKind == AnimName::kKickHeat)  return "必殺技発動中";

    return "アニメーションなし";
}
#endif
