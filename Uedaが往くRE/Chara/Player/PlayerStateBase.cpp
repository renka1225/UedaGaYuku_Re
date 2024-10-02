#include "PlayerStateBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pPlayer"></param>
PlayerStateBase::PlayerStateBase(std::shared_ptr<Player> pPlayer):
	m_analogInput({}),
	m_analogX(0),
	m_analogY(0)
{
	m_pPlayer = pPlayer;
}
