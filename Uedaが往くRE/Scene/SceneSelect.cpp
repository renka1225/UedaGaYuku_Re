#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Message.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

namespace
{
	// �����F
	constexpr int kTextColorW = 0xffffff; // ��
	constexpr int kTextColorR = 0xff0000; // ��
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneSelect::SceneSelect()
{
	m_select = SceneSelect::kMain;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneSelect::~SceneSelect()
{
}

/// <summary>
/// ������
/// </summary>
void SceneSelect::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// �I����ԍX�V
	UpdateSelect(input, SelectScene::kSelectNum);

	// �J��
	if (input.IsTriggered("OK"))
	{
		if (m_select == SelectScene::kMain)
		{
			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>();
		}
	}
	else if (input.IsTriggered("back"))
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneSelect::Draw()
{
#ifdef _DEBUG
	int mainColor = TextColor::kColorW;
	int optionColor = TextColor::kColorW;

	if (m_select == SelectScene::kMain) mainColor = TextColor::kColorR;
	if (m_select == SelectScene::kOption) optionColor = TextColor::kColorR;

	DrawString(0, 60, Message::GetInstance().Get_c("MSG_DEBUG_PLAYING"), mainColor);
	DrawString(0, 80, Message::GetInstance().Get_c("MSG_DEBUG_OPTION"), optionColor);

	DrawSceneText("MSG_DEBUG_SELECT"); // �V�[�����\��
#endif
}

