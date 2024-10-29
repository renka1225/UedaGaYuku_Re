#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"

void PlayerStateGuard::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kGuard);
}

void PlayerStateGuard::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
	m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// ボタンを離した場合
	if (input.IsReleased(InputId::kGuard))
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
}
