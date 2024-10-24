﻿#include "Game.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"

void PlayerStateRun::Init()
{
	m_pPlayer->ChangeAnim(AnimName::kRun);
}

void PlayerStateRun::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);

    VECTOR upMoveVec;		                            // 上ボタンを入力をしたときの移動方向ベクトル
    VECTOR leftMoveVec;	                                // 左ボタンを入力をしたときの移動方向ベクトル
    VECTOR moveVec = VGet(static_cast<float>(-m_analogX), 0.0f, static_cast<float>(m_analogY)); // 移動ベクトル

    // プレイヤーの移動方向ベクトルを求める
    upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    upMoveVec.y = 0.0f;
    leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // ベクトルの正規化
    upMoveVec = VNorm(upMoveVec);
    leftMoveVec = VNorm(leftMoveVec);

    float rate = VSize(moveVec) / 1000.0f; // ベクトルの長さを0.0～1.0の割合に変換する
    moveVec = VScale(VNorm(moveVec), m_pPlayer->GetStatus().runSpeed * rate);

    // 移動方向を決定する
    MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
    moveVec = VTransform(moveVec, mtx);

    m_pPlayer->Move(moveVec, stage);   // 移動情報を反映する

    // 移動中にAボタンを離した場合
    if (input.IsReleased(InputId::kA) && (m_analogX != 0 || m_analogY != 0))
    {
        // StateをWalkに変更する
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }
    // スティックを倒していない場合
    else if (m_analogX == 0 && m_analogY == 0)
    {
        // StateをIdleに変更する
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }
}