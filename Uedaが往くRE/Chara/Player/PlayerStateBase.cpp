#include "Game.h"
#include "Input.h"
#include "SceneBase.h"
#include "PlayerStateAttack.h"
#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(std::shared_ptr<Player> pPlayer):
	m_analogInput({}),
	m_analogX(0),
	m_analogY(0)
{
	m_pPlayer = pPlayer;
}

void PlayerStateBase::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
{
	GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // アナログスティックの入力状態

	// 攻撃のボタンが押された場合
	if (input.IsTriggered(InputId::kPunch))
	{
		// StateをAttackに変更する
		m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);

		// 押されたボタンによって状態を変更する
		if (input.IsTriggered(InputId::kPunch))
		{
			state->Init("Punch");
		}
		else if (input.IsTriggered(InputId::kKick))
		{
			state->Init("Kick");
		}
		return;
	}
}
