#include "DxLib.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"

/// <summary>
/// ������
/// </summary>
void PlayerStateWalk::Init()
{
	m_pPlayer->ChangeAnim("Walk");
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
void PlayerStateWalk::Update(const Input& input, const Camera& camera)
{
    GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // �A�i���O�X�e�B�b�N�̓��͏��

    VECTOR upMoveVec;		                            // ��{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR leftMoveVec;	                                // ���{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR moveVec = VGet(m_analogX, 0.0f, m_analogY);  // �ړ��x�N�g��

    // A�{�^���𒷉������Ă���ꍇ
    if (input.IsPressing("A") && (m_analogX != 0.0f || m_analogY != 0.0f))
    {
        // State��Run�ɕύX����
        m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
        state->Init();
        return;
    }
	// �{�^���𗣂����ꍇ
	else if (m_analogX == 0.0f && m_analogY == 0.0f)
	{
		// State��Idle�ɕύX����
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

    // �v���C���[�̈ړ������x�N�g�������߂�
    upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    upMoveVec.y = 0.0f;
    leftMoveVec = VCross(upMoveVec, VGet(0.0f, -1.0f, 0.0f));

    // �x�N�g���̐��K��
    upMoveVec = VNorm(upMoveVec);
    leftMoveVec = VNorm(leftMoveVec);
    leftMoveVec = VAdd(leftMoveVec, VScale(leftMoveVec, -1));

    float rate = VSize(moveVec) / 1000.0f; // �x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����

    moveVec = VScale(VNorm(moveVec), m_pPlayer->GetStatus().walkSpeed * rate);

    // �ړ����������肷��
    MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
    moveVec = VTransform(moveVec, mtx);


    m_pPlayer->Move(moveVec);   // �ړ����𔽉f����

    printfDx("%.2f, (X:%.2f, Y:%.2f, Z:%.2f)\n", camera.GetAngleH(), moveVec.x, moveVec.y, moveVec.z);


#ifdef false
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
#endif
}
