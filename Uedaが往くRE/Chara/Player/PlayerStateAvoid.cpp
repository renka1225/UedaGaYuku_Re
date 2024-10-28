#include "Game.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"

void PlayerStateAvoid::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kAvoid);
	m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kAvoid) / m_pPlayer->GetAnimSpeed();
}

void PlayerStateAvoid::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

	m_animEndTime--;
	if (m_animEndTime < 0.0f)
	{
		// StateをIdleに変更する
		m_pPlayer->SetIsAttack(false);
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

	if (VSize(m_moveVec) > 0.0f) return; // すでに回避中の場合更新しない

	// スティック入力中は入力方向に移動
	if (m_analogX != 0 || m_analogY != 0)
	{
		m_moveVec = VGet(static_cast<float>(-m_analogX), 0.0f, static_cast<float>(m_analogY));
		// 移動方向を決定する
		MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
		m_moveVec = VTransform(m_moveVec, mtx);
	}
	// スティック入力中でない場合は後ろに移動
	else
	{
		m_moveVec = VScale(m_pPlayer->GetDir(), -1.0f);
	}

	m_moveVec = VScale(VNorm(m_moveVec), m_pPlayer->GetStatus().avoidDist);
	m_moveVec.y = 0.0f;
	m_pPlayer->Move(m_moveVec, stage, false);   // 移動情報を反映する
}
