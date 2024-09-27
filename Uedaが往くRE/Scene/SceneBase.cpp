#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Message.h"
#include "Vec2.h"
#include "SceneBase.h"

namespace
{
	const Vec2 kCurrentScenePos = { 0.0f, 0.0f }; // ���݂̃V�[�����\���ʒu
}

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

#ifdef _DEBUG
/// <summary>
/// ���݂̃V�[�����f�o�b�O�\������
/// </summary>
void SceneBase::DrawSceneText(const char* sceneName)
{
	DrawStringF(kCurrentScenePos.x, kCurrentScenePos.y, Message::GetInstance().Get_c(sceneName), TextColor::kColorR);
}
#endif

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
