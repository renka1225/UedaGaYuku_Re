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
	m_pPlayer->ChangeAnim("Run");
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
void PlayerStateRun::Update(const Input& input, const Camera& camera)
{
    GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // �A�i���O�X�e�B�b�N�̓��͏��

    VECTOR upMoveVec;		                            // ��{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR leftMoveVec;	                                // ���{�^������͂������Ƃ��̈ړ������x�N�g��
    VECTOR moveVec = VGet(-m_analogX, 0.0f, m_analogY); // �ړ��x�N�g��

    // �v���C���[�̈ړ������x�N�g�������߂�
    upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
    upMoveVec.y = 0.0f;
    leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

    // �x�N�g���̐��K��
    upMoveVec = VNorm(upMoveVec);
    leftMoveVec = VNorm(leftMoveVec);

    float rate = VSize(moveVec) / 1000.0f; // �x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
    moveVec = VScale(VNorm(moveVec), m_pPlayer->GetStatus().runSpeed * rate);

    // �ړ����������肷��
    MATRIX mtx = MGetRotY(camera.GetAngleH() - DX_PI_F / 2);
    moveVec = VTransform(moveVec, mtx);

    m_pPlayer->Move(moveVec);   // �ړ����𔽉f����

    // �ړ�����A�{�^���𗣂����ꍇ
    if (input.IsReleased("A") && (m_analogX != 0.0f || m_analogY != 0.0f))
    {
        // State��Walk�ɕύX����
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }
    // �X�e�B�b�N��|���Ă��Ȃ��ꍇ
    else if (m_analogX == 0.0f && m_analogY == 0.0f)
    {
        // State��Idle�ɕύX����
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();
        return;
    }

}