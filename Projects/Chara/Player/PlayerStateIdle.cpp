#include "Input.h"
#include "LoadCsv.h"
#include "Player.h"
#include "PlayerStateAttack.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kIdleStand);
}

void PlayerStateIdle::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

	if (!m_pPlayer->GetIsPossibleMove()) return;

	// 移動ボタンが押されている場合
	if (input.IsPressing(InputId::kA) && (m_analogX != 0 || m_analogY != 0))
	{
		// StateをRunに変更する
		std::shared_ptr<PlayerStateRun> state = std::make_shared<PlayerStateRun>(m_pPlayer);
		m_nextState = state;
		state->Init();
		return;
	}
	else if (m_analogX != 0 || m_analogY != 0)
	{
		// StateをWalkに変更する
		std::shared_ptr<PlayerStateWalk> state = std::make_shared<PlayerStateWalk>(m_pPlayer);
		m_nextState = state;
		state->Init();
		return;
	}
}