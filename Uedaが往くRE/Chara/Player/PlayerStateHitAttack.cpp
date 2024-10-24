#include "Player.h"
#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Init()
{
	m_pPlayer->ChangeAnim("Damage");
}

void PlayerStateHitAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy)
{
}