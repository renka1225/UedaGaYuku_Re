#include "EnemyBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

// 定数
namespace
{
    constexpr float kAttackEndTime = 60;
}

PlayerStateAttack::PlayerStateAttack(std::shared_ptr<Player> player):
    PlayerStateBase(player),
    m_attackEndTime(kAttackEndTime),
    m_isAttackEnd(false)
{
}

void PlayerStateAttack::Init(std::string inputId)
{
	m_pPlayer->ChangeAnim(inputId);
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        // StateをIdleに変更する
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
        if(pEnemy != nullptr) pEnemy->OnDamage(5);
    }
}
