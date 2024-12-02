#include "Player.h"
#include "PLayerStateIdle.h"
#include "PlayerStateGrab.h"

PlayerStateGrab::PlayerStateGrab(std::shared_ptr<Player> player):
    PlayerStateBase(player)
{
}

void PlayerStateGrab::Init(std::string grabKind)
{
    m_grabKind = grabKind;

    if (m_grabKind == "enemy")
    {
        m_pPlayer->ChangeAnim(AnimName::kGrabEnemy);
        m_animEndTime = 10.0f;
    }
    else
    {
        m_pPlayer->ChangeAnim(AnimName::kGrabOneHandWeapon);
        m_animEndTime = 10.0f;
    }
}

void PlayerStateGrab::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する

    m_animEndTime--;
    if (m_animEndTime < 0.0f)
    {
        // StateをIdleに変更する
        std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
        m_nextState = state;
        state->Init();
        return;
    }
}

#ifdef _DEBUG
std::string PlayerStateGrab::GetStateName()
{
    if (m_grabKind == "enemy")
    {
        return "敵掴み";
    }
    else if (m_grabKind == "oneHandWeapon")
    {
        return "片手武器掴み";
    }
    else
    {
        return "掴み";
    }
}
#endif
