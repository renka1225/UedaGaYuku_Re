#include "Player.h"
#include "PLayerStateIdle.h"
#include "PlayerStateGrab.h"

// 定数
namespace
{
    constexpr float kAttackEndTime = 10;
}


PlayerStateGrab::PlayerStateGrab(std::shared_ptr<Player> player):
    PlayerStateBase(player),
    m_animTime(kAttackEndTime),
    m_isAnimEnd(false)
{
}

void PlayerStateGrab::Init()
{
	m_pPlayer->ChangeAnim("Grab");
}

void PlayerStateGrab::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

    // アニメーション終了した場合
    if (m_isAnimEnd)
    {
        // StateをIdleに変更する
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }
    else
    {
        m_animTime--;
        if (m_animTime < 0)
        {
            m_isAnimEnd = true;
        }
    }
}
