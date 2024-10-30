#include "Game.h"
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

    if (m_attackKind == AnimName::kPunchStrong)
    {
        m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kPunchStrong);
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
        std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
        m_nextState = state;
        state->Init();
        return;
    }
    else
    {
        m_animEndTime--;
        if (m_animEndTime < 0.0f) m_isAttackEnd = true;
        
        for (auto& enemy : pEnemy)
        {
            // 特定の状態の場合はスキップする
            bool isSkip = enemy == nullptr || enemy->GetIsInvincible();
            if (isSkip) continue;

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
            else if (m_attackKind == AnimName::kPunchStrong)
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
}

#ifdef _DEBUG
std::string PlayerStateAttack::GetStateName()
{
    if (m_attackKind == AnimName::kPunchStrong)
    {
        return "パンチ中";
    }
    else if (m_attackKind == AnimName::kKick)
    {
        return "キック中";
    }

    return "攻撃中";
}
#endif
