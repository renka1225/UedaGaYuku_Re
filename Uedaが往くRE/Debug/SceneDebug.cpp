#include "DxLib.h"
#include "Input.h"
#include "Message.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneDebug.h"

// �萔
namespace
{
	constexpr int kTextColor = 0xffffff;		// �����F
	constexpr int kSelectTextColor = 0xff0000;	// �I�𒆂̕����F
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneDebug::SceneDebug()
{
	m_select = SelectScene::kTitle;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneDebug::Update(Input& input)
{
	// �I����ԍX�V
	UpdateSelect(input, kSelectNum);

	// �J��
	if (input.IsTriggered("OK"))
	{
		if (m_select == SelectScene::kTitle)
		{
			return std::make_shared<SceneTitle>();
		}
		else if (m_select == SelectScene::kSelect)
		{
			return std::make_shared<SceneSelect>();
		}
		else if (m_select == SelectScene::kMain)
		{
			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>();
		}
	}

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneDebug::Draw()
{
	int titleColor = kTextColor;
	int selectColor = kTextColor;
	int playColor = kTextColor;
	int optionColor = kTextColor;

	if (m_select == SelectScene::kTitle) titleColor = kSelectTextColor;
	if (m_select == SelectScene::kSelect) selectColor = kSelectTextColor;
	if (m_select == SelectScene::kMain) playColor = kSelectTextColor;
	if (m_select == SelectScene::kOption) optionColor = kSelectTextColor;

	DrawString(0, 20, Message::GetInstance().Get_c("MSG_DEBUG_TITLE"), titleColor);
	DrawString(0, 40, Message::GetInstance().Get_c("MSG_DEBUG_SELECT"), selectColor);
	DrawString(0, 60, Message::GetInstance().Get_c("MSG_DEBUG_PLAYING"), playColor);
	DrawString(0, 80, Message::GetInstance().Get_c("MSG_DEBUG_OPTION"), optionColor);
}