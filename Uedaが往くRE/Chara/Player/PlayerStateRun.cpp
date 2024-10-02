#include "DxLib.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"

/// <summary>
/// 初期化
/// </summary>
void PlayerStateRun::Init()
{
	m_pPlayer->ChangeAnim("Run");
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力処理</param>
void PlayerStateRun::Update(const Input& input, const Camera& camera)
{
    GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // アナログスティックの入力状態

    VECTOR upMoveVec;		                            // 上ボタンを入力をしたときの移動方向ベクトル
    VECTOR leftMoveVec;	                                // 左ボタンを入力をしたときの移動方向ベクトル
    VECTOR moveVec = VGet(-m_analogX, 0.0f, m_analogY); // 移動ベクトル

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

    m_pPlayer->Move(moveVec);   // 移動情報を反映する

    // 移動中にAボタンを離した場合
    if (input.IsReleased("A") && (m_analogX != 0.0f || m_analogY != 0.0f))
    {
        // StateをWalkに変更する
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }
    // スティックを倒していない場合
    else if (m_analogX == 0.0f && m_analogY == 0.0f)
    {
        // StateをIdleに変更する
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }

}