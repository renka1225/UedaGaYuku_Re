#include "Input.h"
#include "Player.h"
#include "PlayerStateIdle.h"

/// <summary>
/// ������
/// </summary>
void PlayerStateIdle::Init()
{
	//m_pPlayer->ChangeAnim("Idle");
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
void PlayerStateIdle::Update(Input input)
{
	// �ړ��{�^����������Ă���ꍇ
	if (input.IsPressing("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down"))
	{
		// State��Move�ɕύX����
		//m_nextState = std::make_shared<PlayStateWalk>(m_pPlayer);
	}
}
