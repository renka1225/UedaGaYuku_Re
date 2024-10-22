#include "Game.h"
#include "EnemyBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

// 定数
namespace
{
    constexpr float kPunchEndTime = 30;
    constexpr float kKickEndTime = 120;
}

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
        m_attackEndTime = kPunchEndTime;
    }
    else if (m_attackKind == AnimName::kKick)
    {
        m_attackEndTime = kKickEndTime;
    }
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
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

        if (m_attackEndTime < 0)
        {
            m_isAttackEnd = true;
        }

        // 敵にダメージを与える
        if (pEnemy != nullptr)
        {
            // プレイヤーの攻撃と敵の当たり判定を取得
            if (m_attackKind == AnimName::kPunchStrong)
            {
                bool isHitPunchCol = pEnemy->CheckHitPunchCol(*m_pPlayer, m_pPlayer->GetCol().armStartPos, m_pPlayer->GetCol().armEndPos, m_pPlayer->GetCol().armRadius);
                if (isHitPunchCol)
                {
                    pEnemy->OnDamage(5);
                }
            }
            else if (m_attackKind == AnimName::kKick)
            {
                bool isHitKickCol = pEnemy->CheckHitKickCol(*m_pPlayer, m_pPlayer->GetCol().legStartPos, m_pPlayer->GetCol().legEndPos, m_pPlayer->GetCol().legRadius);
                if (isHitKickCol)
                {
                    pEnemy->OnDamage(10);
                }
            }
        }
    }
}

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
