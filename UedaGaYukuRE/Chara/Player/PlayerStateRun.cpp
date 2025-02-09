﻿#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"

void PlayerStateRun::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kRun);
}

void PlayerStateRun::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

    // 移動できない状態の場合は飛ばす
    if (!m_pPlayer->GetIsPossibleMove()) return;

    m_moveVec = VGet(static_cast<float>(-m_analogX), 0.0f, static_cast<float>(m_analogY)); // 移動ベクトル

    // プレイヤーの移動方向ベクトルを求める
    m_upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    m_upMoveVec.y = 0.0f;
    m_leftMoveVec = VCross(m_upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // ベクトルの正規化
    m_upMoveVec = VNorm(m_upMoveVec);
    m_leftMoveVec = VNorm(m_leftMoveVec);

    float rate = VSize(m_moveVec) / 1000.0f; // ベクトルの長さを0.0～1.0の割合に変換する
    m_moveVec = VScale(VNorm(m_moveVec), m_pPlayer->GetStatus().runSpeed * rate);

    // 移動方向を決定する
    MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
    m_moveVec = VTransform(m_moveVec, mtx);

    // 移動中にAボタンを離した場合
    if (input.IsReleased(InputId::kA) && (m_analogX != 0 || m_analogY != 0))
    {
        // StateをWalkに変更する
        std::shared_ptr<PlayerStateWalk> state = std::make_shared<PlayerStateWalk>(m_pPlayer);
        m_nextState = state;
        state->Init();
        return;
    }
    // スティックを倒していない場合
    else if (m_analogX == 0 && m_analogY == 0)
    {
        // StateをIdleに変更する
        std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
        m_nextState = state;
        state->Init();
        return;
    }

    m_pPlayer->Move(m_moveVec, stage);   // 移動情報を反映する
}