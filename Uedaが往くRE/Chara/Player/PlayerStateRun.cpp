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
	//m_pPlayer->ChangeAnim("Walk");
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力処理</param>
void PlayerStateRun::Update(const Input& input, const Camera& camera)
{
	// Aボタンを離した場合
    if (input.IsReleased("A") && (input.IsReleased("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down")))
    {
        // StateをWalkに変更する
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }
    // 移動ボタンを離した場合
	else if (input.IsReleased("left") || input.IsReleased("right") || input.IsReleased("up") || input.IsReleased("down"))
	{
		// StateをIdleに変更する
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

    VECTOR upMoveVec;		                 // 上ボタンを入力をしたときの移動方向ベクトル
    VECTOR leftMoveVec;	                     // 左ボタンを入力をしたときの移動方向ベクトル
    VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // 移動ベクトル

    // プレイヤーの移動方向ベクトルを求める
    // 上ボタンを押したとき
    upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    upMoveVec.y = 0.0f;

    // 左ボタンを押したとき
    leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // ベクトルの正規化
    upMoveVec = VNorm(upMoveVec);
    leftMoveVec = VNorm(leftMoveVec);

    // ボタンを押した場合
    if (input.IsPressing("left"))
    {
        moveVec = VAdd(moveVec, leftMoveVec);
    }
    if (input.IsPressing("right"))
    {
        moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));
    }
    if (input.IsPressing("up"))
    {
        moveVec = VAdd(moveVec, upMoveVec);
    }
    if (input.IsPressing("down"))
    {
        moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));
    }

    moveVec = VScale(moveVec, m_pPlayer->GetStatus().runSpeed);

    // 移動情報を反映する
    m_pPlayer->Move(moveVec);

}