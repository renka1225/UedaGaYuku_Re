#include "DxLib.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"

/// <summary>
/// ������
/// </summary>
void PlayerStateRun::Init()
{
	//m_pPlayer->ChangeAnim("Walk");
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
void PlayerStateRun::Update(const Input& input, const Camera& camera)
{
	// A�{�^���𗣂����ꍇ
    if (input.IsReleased("A") && (input.IsReleased("left") || input.IsPressing("right") || input.IsPressing("up") || input.IsPressing("down")))
    {
        // State��Walk�ɕύX����
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }
    // �ړ��{�^���𗣂����ꍇ
	else if (input.IsReleased("left") || input.IsReleased("right") || input.IsReleased("up") || input.IsReleased("down"))
	{
		// State��Idle�ɕύX����
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

    VECTOR upMoveVec;		                 // ��{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR leftMoveVec;	                     // ���{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // �ړ��x�N�g��

    // �v���C���[�̈ړ������x�N�g�������߂�
    // ��{�^�����������Ƃ�
    upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    upMoveVec.y = 0.0f;

    // ���{�^�����������Ƃ�
    leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // �x�N�g���̐��K��
    upMoveVec = VNorm(upMoveVec);
    leftMoveVec = VNorm(leftMoveVec);

    // �{�^�����������ꍇ
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

    // �ړ����𔽉f����
    m_pPlayer->Move(moveVec);

}