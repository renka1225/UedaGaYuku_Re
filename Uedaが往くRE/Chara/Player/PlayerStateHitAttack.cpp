#include "Player.h"
#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Init()
{
	m_pPlayer->ChangeAnim("Damage");
}

void PlayerStateHitAttack::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
{
}