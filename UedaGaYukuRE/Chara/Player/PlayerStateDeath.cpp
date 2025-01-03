#include "Player.h"
#include "PlayerStateDeath.h"

void PlayerStateDeath::Init()
{
    m_pPlayer->ChangeAnim(AnimName::kGrabTwoHandWeapon);
    m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kDown);
}

void PlayerStateDeath::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

	m_animEndTime--;
	if (m_animEndTime < 0.0f)
	{
		m_pPlayer->SetIsDead(true);
		return;
	}
}
