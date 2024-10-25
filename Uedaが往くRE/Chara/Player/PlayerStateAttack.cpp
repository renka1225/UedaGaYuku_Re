#include "Game.h"
#include "Weapon.h"
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

PlayerStateAttack::PlayerStateAttack(std::shared_ptr<Player> player):
    PlayerStateBase(player),
    m_attackEndTime(0),
    m_isAttackEnd(false)
{
}

void PlayerStateAttack::Init(std::string attackName)
{
    m_attackKind = attackName;
	m_pPlayer->ChangeAnim(m_attackKind);

    if (m_attackKind == AnimName::kPunchStrong)
    {
        m_attackEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kPunchStrong);
    }
    else if (m_attackKind == AnimName::kKick)
    {
        m_attackEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kKick);
    }
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        weapon.SetIsHitAttack(false);

        // StateをIdleに変更する
        m_pPlayer->SetIsAttack(false);
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }
    else
    {
        m_attackEndTime--;
        if (m_attackEndTime < 0) m_isAttackEnd = true;
        
        if (pEnemy == nullptr) return;

        // 武器掴み中の場合
        if (m_pPlayer->GetIsGrabWeapon())
        {
            // 武器と敵の当たり判定を取得
            bool isHitWeaponCol = weapon.CheckWeaopnCol(pEnemy->GetCol(pEnemy->GetEnemyNumber()), *m_pPlayer);
            if (isHitWeaponCol)
            {
                pEnemy->OnDamage(20);
                weapon.DecrementDurability();
            }
        }
        // パンチ攻撃
        else if (m_attackKind == AnimName::kPunchStrong)
        {
            // パンチ攻撃と敵の当たり判定を取得
            bool isHitPunchCol = pEnemy->CheckHitPunchCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), pEnemy->GetEnemyNumber());
            if (isHitPunchCol)
            {
                pEnemy->OnDamage(5);
            }
        }
        // キック攻撃
        else if (m_attackKind == AnimName::kKick)
        {
            // キック攻撃と敵の当たり判定を取得
            bool isHitKickCol = pEnemy->CheckHitKickCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), pEnemy->GetEnemyNumber());
            if (isHitKickCol)
            {

                pEnemy->OnDamage(10);
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
