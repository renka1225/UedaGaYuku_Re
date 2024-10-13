#include "Player.h"
#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Init()
{
	m_pPlayer->ChangeAnim("Damage");
}

void PlayerStateHitAttack::Update(const Input& input, const Camera& camera, Stage& stage)
{
}

int PlayerStateHitAttack::OnDamage()
{
	int damage = 0;

	return damage;
}
