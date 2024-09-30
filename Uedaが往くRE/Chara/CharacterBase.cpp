#include "DebugDraw.h"
#include "CsvLoad.h"
#include "CharacterBase.h"

// �萔
namespace
{
	// �A�j���[�V�������
	constexpr float kAnimBlendMax = 1.0f;	 // �A�j���[�V�����u�����h�̍ő�l
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterBase::CharacterBase():
	m_animData(),
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_prevPlayAnim(-1),
	m_animPlaySpeed(0.0f),
	m_currentAnimTime(0.0f),
	m_prevAnimTime(0.0f),
	m_totalAnimTime(0.0f),
	m_animBlendRate(0.0f)
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
	//LoadAnimData(CsvLoad::GetInstance.LoadAnimData());
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
		MV1DetachAnim(m_modelHandle, static_cast<int>(m_prevAnimTime));
	}

	// �V���ɃA�j���[�V�������A�^�b�`����
	m_prevAnimTime = MV1AttachAnim(m_modelHandle, m_animData.number);
	//�A�j���[�V�����̑��Đ����Ԃ�ݒ�
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData.number);

	//�A�j���̍Đ����x��ݒ�
	m_animPlaySpeed = m_animData.playSpeed;
}


/// <summary>
/// �A�j���[�V�������X�V
/// </summary>
void CharacterBase::UpdateAnim()
{
	// �u�����h����1�ȉ��̏ꍇ
	//if (m_animBlendRate < kAnimBlendMax)
	//{
	//	m_animBlendRate += m_animPlaySpeed;
	//	m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	//}

	// �A�j���[�V�����̍Đ��t���[����i�߂�
	m_currentAnimTime += m_animPlaySpeed;

	if (m_currentAnimTime > m_totalAnimTime)
	{
		m_currentAnimTime = 0.0f;
		m_animPlaySpeed = 0.0f;
	}

	// �Đ����Ԃ��X�V
	MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimTime);
}
