#include "Game.h"
#include "Input.h"
#include "SceneBase.h"
#include "Weapon.h"
#include "Player.h"
#include "PlayerStateAttack.h"
#include "PlayerStateGrab.h"
#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(std::shared_ptr<Player> pPlayer):
	m_analogInput({}),
	m_analogX(0),
	m_analogY(0)
{
	m_pPlayer = pPlayer;
}

void PlayerStateBase::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy)
{
	GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // アナログスティックの入力状態

	// 攻撃のボタンが押された場合
	if (input.IsTriggered(InputId::kPunch) || input.IsTriggered(InputId::kKick))
	{
		// 攻撃中は再度攻撃できないようにする
		if (m_pPlayer->GetIsAttack()) return;

		// StateをAttackに変更する
		m_pPlayer->SetIsAttack(true);
		m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);

		// 押されたボタンによって状態を変更する
		if (input.IsTriggered(InputId::kPunch))
		{
			state->Init(AnimName::kPunchStrong);
		}
		else if (input.IsTriggered(InputId::kKick))
		{
			state->Init(AnimName::kKick);
		}
		return;
	}

	// 掴みのボタンが押されたとき
	if (input.IsTriggered(InputId::kGrab))
	{
		if (m_pPlayer->GetIsPossibleGrabWeapon())
		{
			// 武器を掴んでいない場合
			if (!m_pPlayer->GetIsGrabWeapon())
			{
				m_pPlayer->SetIsGrabWeapon(true); // 武器を掴む

				// StateをGrabに変更する
				m_nextState = std::make_shared<PlayerStateGrab>(m_pPlayer);
				auto state = std::dynamic_pointer_cast<PlayerStateGrab>(m_nextState);
				state->Init();
			}
			// すでに武器を掴んでいる場合
			else
			{
				m_pPlayer->SetIsGrabWeapon(false); // 武器を離す
			}
		}
	
		
	}
}
