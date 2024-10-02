#include "DebugDraw.h"
#include "LoadCsv.h"
#include "CharacterBase.h"

// �萔
namespace
{
	// �A�j���[�V�������
	constexpr float kAnimBlendMax = 1.0f;	 // �A�j���[�V�����u�����h�̍ő�l
	constexpr float kAnimBlendSpeed = 0.2f;	 // �A�j���[�V�����u�����h�̕ω����x
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterBase::CharacterBase():
	m_animData(),
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_prevPlayAnim(-1),
	m_animBlendRate(0.0f),
	m_animPlaySpeed(0.0f),
	m_currentAnimTime(0.0f),
	m_prevAnimTime(0.0f),
	m_totalAnimTime(0.0f),
	m_animLoopStartTime(0.0f),
	m_animLoopEndTime(0.0f),
	m_isLoopAnim(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
void CharacterBase::Init()
{
	LoadCsv::GetInstance().LoadAnimData(m_animData);
}

/// <summary>
/// �X�V
/// </summary>
void CharacterBase::Update()
{
}

/// <summary>
/// �`��
/// </summary>
void CharacterBase::Draw()
{
	MV1DrawModel(m_modelHandle);
}

/// <summary>
/// �A�j���[�V������ύX����
/// </summary>
/// <param name="animName">�A�j���[�V������</param>
void CharacterBase::ChangeAnim(std::string animName)
{
	// �O�̃A�j���[�V�������f�^�b�`����
	if (m_prevAnimTime != -1)
	{
		MV1DetachAnim(m_modelHandle, static_cast<int>(m_prevPlayAnim));
		m_prevPlayAnim = -1;
	}

	// �Đ����̃A�j���[�V������1�O�Ɉړ�����
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimTime = m_currentAnimTime;

	// �A�j���[�V������ݒ�
	m_animPlaySpeed = m_animData[animName].playSpeed;
	m_animLoopStartTime = m_animData[animName].loopFrame;
	m_animLoopEndTime = m_animData[animName].endFrame;

	// ���[�v�I���t���[����0�łȂ��ꍇ�A���[�v�t���O�𗧂Ă�
	if (m_animLoopEndTime > 0)
	{
		m_isLoopAnim = true;
	}

	// �V���ɃA�j���[�V�������A�^�b�`����
	m_currentPlayAnim = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	m_currentAnimTime = 0.0f;
	//�A�j���[�V�����̑��Đ����Ԃ�ݒ�
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].number);

	// �u�����h����prev���L���łȂ��ꍇ�A1.0�ɂ���
	if (m_prevPlayAnim == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else
	{
		m_animBlendRate = 0.0f;
	}
}

/// <summary>
/// �A�j���[�V�������X�V
/// </summary>
void CharacterBase::UpdateAnim()
{
	// �u�����h����1�ȉ��̏ꍇ
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimBlendSpeed;
		m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	}

	// �A�j���[�V�����̍Đ����Ԃ�i�߂�
	m_currentAnimTime += m_animPlaySpeed;

	// �A�j���[�V�������J��Ԃ��s����ꍇ
	if (m_isLoopAnim)
	{
		if (m_currentAnimTime > m_animLoopEndTime)
		{
			m_currentAnimTime = m_animLoopStartTime;
		}
	}

	// �A�j���[�V�����̑��Đ����Ԃ𒴂����ꍇ
	if (m_currentAnimTime > m_totalAnimTime)
	{
		m_currentAnimTime = 0.0f;
		m_animPlaySpeed = 0.0f;
	}

	// �Đ����Ԃ��X�V
	MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimTime);
	// �A�j���[�V�����̃u�����h����ݒ肷��
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevPlayAnim, kAnimBlendMax - m_animBlendRate);
}
