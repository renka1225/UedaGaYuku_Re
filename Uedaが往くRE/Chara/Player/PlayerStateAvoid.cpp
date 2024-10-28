#include "Game.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"

void PlayerStateAvoid::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kAvoid);
	m_animEndTime = m_pPlayer->GetAnimTotalTime(AnimName::kAvoid);
}

void PlayerStateAvoid::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

	m_moveVec = VGet(static_cast<float>(-m_analogX), 0.0f, static_cast<float>(m_analogY)); // 移動ベクトル

	// 移動方向を決定する
	MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
	m_moveVec = VTransform(m_moveVec, mtx);

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
	
	// 移動ベクトルを設定する
	// 方向キーを入力中は入力方向へ移動する
	if (m_analogX != 0 || m_analogY != 0)
	{
		m_moveVec = VScale(m_moveVec, m_pPlayer->GetStatus().avoidDist);
		m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する
	}
	// 方向キーが入力されていない場合は後ろへ移動する
	else
	{
		m_moveVec =  VScale(VScale(m_moveVec, -1.0f), m_pPlayer->GetStatus().avoidDist);
		m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する
	}
}
