#include "Input.h"
#include "LoadCsv.h"
#include "Player.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

/// <summary>
/// ������
/// </summary>
void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim("StandIdle");
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
void PlayerStateIdle::Update(const Input& input, const Camera& camera)
{
	// �ړ��{�^����������Ă���ꍇ
	if (input.IsPressing("A") && (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down")))
	{
		// State��Run�ɕύX����
		m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
		state->Init();
		return;
	}
	else if (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down"))
	{
		// State��Walk�ɕύX����
		m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}

	m_pPlayer->UpdateAnim(); // �A�j���[�V�������X�V
}