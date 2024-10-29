#include "Game.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"

void PlayerStateWalk::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kWalk);
}

void PlayerStateWalk::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

    m_moveVec = VGet(static_cast<float>(-m_analogX), 0.0f, static_cast<float>(m_analogY)); // 移動ベクトル

    // プレイヤーの移動方向ベクトルを求める
    m_upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    m_leftMoveVec = VCross(m_upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // ベクトルの正規化
    m_upMoveVec = VNorm(m_upMoveVec);
    m_leftMoveVec = VNorm(m_leftMoveVec);

    float rate = VSize(m_moveVec) / 1000.0f; // ベクトルの長さを0.0～1.0の割合に変換する
    m_moveVec = VScale(VNorm(m_moveVec), m_pPlayer->GetStatus().walkSpeed * rate);

    // 移動方向を決定する
    MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
    m_moveVec = VTransform(m_moveVec, mtx);

    // 移動中にAボタンを長押ししている場合
    //if (input.IsPressing(InputId::kA) && (m_analogX != 0 || m_analogY != 0))
    //{
    //    // StateをRunに変更する
    //    m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
    //    auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
    //    state->Init();
    //    return;
    //}
    // スティックを倒していない場合
	if (m_analogX == 0 && m_analogY == 0)
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

    m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する
}
