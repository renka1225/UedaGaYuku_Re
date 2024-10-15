#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

PlayerStateAttack::PlayerStateAttack(std::shared_ptr<Player> player):
    PlayerStateBase(player),
    m_isAttackEnd(false)
{
}

void PlayerStateAttack::Init()
{
	m_pPlayer->ChangeAnim("Attack");
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage)
{
	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        // StateをIdleに変更する
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }
}
