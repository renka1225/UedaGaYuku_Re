#include "DebugDraw.h"
#include "CharacterBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterBase::CharacterBase():
	m_status(),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_angle(0.0f),
	m_hp(0.0f),
	m_modelHandle(-1)
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
}
