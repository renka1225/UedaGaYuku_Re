#include "Player.h"
#include "PlayerStateAttack.h"

void PlayerStateAttack::Init()
{
	m_pPlayer->ChangeAnim("Attack");
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage)
{
}
