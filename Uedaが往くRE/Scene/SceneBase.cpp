#include "SceneBase.h"
#include "Input.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneBase::SceneBase():
	m_select(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneBase::~SceneBase()
{
}

/// <summary>
/// �I����ԍX�V
/// </summary>
/// <param name="input">���͏���</param>
/// <param name="selectNum">�I�����̐�</param>
void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// �I����Ԃ�1������
	if (input.IsTriggered("down"))
	{
		m_select = (m_select + 1) % selectNum;
	}
	// �I����Ԃ�1�グ��
	if (input.IsTriggered("up"))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
	}
}
