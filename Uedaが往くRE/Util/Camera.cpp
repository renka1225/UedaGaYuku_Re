#include "DxLib.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"
#include "Camera.h"
#include <cmath>

// �萔
namespace
{
	constexpr float kNear = 1.0f;							// �J�����̎�O�N���b�v����
	constexpr float kFar = 5000.0f;							// �J�����̉��N���b�v����
	constexpr float kDist = 20.0f;							// �J��������v���C���[�܂ł̋���
	constexpr float kHeight = 10.0f;						// �J�����̒����_
	constexpr float kAngle = 0.03f;							// �J�����𓮂����p�x
	constexpr float kInitAngleH = -0.6f;					// �J�����̏������s�p�x
	constexpr float kInitAngleV = -0.3f;					// �J�����̏��������p�x
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 1.0f;		// �ŏ��̐����p�x
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 0.6f;	// �ő�̐����p�x
	constexpr float kColSize = 3.0f;						// �J�����̓����蔻��T�C�Y
	constexpr float kHitLength = 0.1f;						// �J�������X�e�[�W�ɓ������������肷�鋗��
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera() :
	m_pos(VGet(0.0f, kHeight, 0.0f)),
	m_target(VGet(0.0f, 0.0f, 0.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV),
	m_rotY(),
	m_rotZ(),
	m_lightHandle(-1)
{
	AnalogInput.Rx = 0;
	AnalogInput.Ry = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
	// �����Ȃ�
}

/// <summary>
/// ������
/// </summary>
void Camera::Init()
{
	m_pos = VGet(0.0f, kHeight, 0.0f);
	m_target = VGet(0.0f, 0.0f, 0.0f);
	m_angleH = kInitAngleH;
	m_angleV = kInitAngleV;
	SetCameraPositionAndTarget_UpVecY(m_pos, m_target);
	SetCameraNearFar(kNear, kFar);
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update(Input& input, const Player& player, const Stage& stage)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &AnalogInput); // ���͏�Ԃ��擾

	// ������
	if (AnalogInput.Rx < 0.0f || input.IsPressing("rotateL"))
	{
		m_angleH -= kAngle;
	}
	// �E����
	if (AnalogInput.Rx > 0.0f || input.IsPressing("rotateR"))
	{
		m_angleH += kAngle;
	}
	// �����
	if (AnalogInput.Ry > 0.0f || input.IsPressing("rotateU"))
	{
		m_angleV -= kAngle;
		m_angleV = std::max(m_angleV, kMaxAngleV);
	}
	// ������
	if (AnalogInput.Ry < 0.0f || input.IsPressing("rotateD"))
	{
		m_angleV += kAngle;
		m_angleV = std::min(kMinAngleV, m_angleV);
	}

	// �J�����̒����_��ݒ肷��
	m_target = VAdd(player.GetPos(), VGet(0.0f, kHeight, 0.0f));
	printfDx("X:%2f,Y:%2f,Z:%2f)\n", m_target.x, m_target.y, m_target.z);

	// �J�����ʒu�␳
	FixCameraPos();
	SetCameraPositionAndTarget_UpVecY(m_pos, m_target);

	// �J�����̓����蔻����`�F�b�N
	//CheckHitCol(stage);

	//�J�����̌��Ă�������Ƀf�B���N�V�������C�g��ݒ肷��
	SetLightDirectionHandle(m_lightHandle, VNorm(VSub(m_target, m_pos)));
}

/// <summary>
/// �J�����ʒu��␳����
/// </summary>
void Camera::FixCameraPos()
{
	m_rotY = MGetRotY(m_angleH);	// ���������̉�]
	m_rotZ = MGetRotZ(m_angleV);	// ���������̉�]

	// �J�����̍��W�����߂�
	// X���ɃJ��������v���C���[�܂ł̋������L�т��x�N�g���𐂒������ɉ�]����(Z����])
	m_pos = VTransform(VGet(-kDist, 0.0f, 0.0f), m_rotZ);
	// ��������(Y����])�ɉ�]����
	m_pos = VTransform(m_pos, m_rotY);
	// �����_�̍��W�𑫂�
	m_pos = VAdd(m_pos, m_target);
}


/// <summary>
/// �����蔻����`�F�b�N����
/// </summary>
void Camera::CheckCameraCol(const Stage& stage)
{
	// �����_����J�����̍��W�܂ł̊ԂɃX�e�[�W�̃|���S�������邩���ׂ�
	float notHitLength = 0.0f;	// �|���S���ɓ�����Ȃ�����
	float hitLength = kDist;	// �|���S���ɓ����鋗��

	do
	{
		// �J�������X�e�[�W�ɓ����邩�e�X�g���鋗��
		// ������Ȃ������Ɠ����鋗���̒��Ԃ����߂�
		float testLength = notHitLength + (hitLength - notHitLength) * 0.5f;
		// ���̃t���[���̃J�������W�����߂�
		auto nextPos = VTransform(VGet(-testLength, 0.0f, 0.0f), m_rotZ);
		nextPos = VTransform(nextPos, m_rotY);
		nextPos = VAdd(nextPos, m_target);

		// �V�������W�ŕǂɓ����邩�e�X�g����
		auto hitResult = MV1CollCheck_Capsule(stage.GetStageHandle(), -1, m_target, nextPos, kColSize);
		int hitNum = hitResult.HitNum;
		MV1CollResultPolyDimTerminate(hitResult);

		// ���������ꍇ
		if (hitNum != 0)
		{
			hitLength = testLength;
			// �J�������W���X�V
			m_pos = nextPos;
		}
		else
		{
			// ������Ȃ�������testLenth�ɕύX����
			notHitLength = testLength;
		}
	} while (hitLength - notHitLength > kHitLength); // hitLength��NoHitLength���\���ɋ߂Â��Ă��Ȃ��ꍇ���[�v����
}
