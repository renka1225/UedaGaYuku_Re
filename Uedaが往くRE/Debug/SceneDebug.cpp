#include "DxLib.h"
#include "Input.h"
#include "Message.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneDebug.h"

// 定数
namespace
{
	// 文字色
	constexpr int kTextColorW = 0xffffff; // 白
	constexpr int kTextColorR = 0xff0000; // 赤
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneDebug::SceneDebug()
{
	m_select = SelectScene::kTitle;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneDebug::Update(Input& input)
{
	// 選択状態更新
	UpdateSelect(input, kSelectNum);

	// 遷移
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
/// 描画
/// </summary>
void SceneDebug::Draw()
{
	int titleColor = kTextColorW;
	int selectColor = kTextColorW;
	int playColor = kTextColorW;
	int optionColor = kTextColorW;

	if (m_select == SelectScene::kTitle) titleColor = kTextColorR;
	if (m_select == SelectScene::kSelect) selectColor = kTextColorR;
	if (m_select == SelectScene::kMain) playColor = kTextColorR;
	if (m_select == SelectScene::kOption) optionColor = kTextColorR;

	DrawString(0, 20, Message::GetInstance().Get_c("MSG_DEBUG_TITLE"), titleColor);
	DrawString(0, 40, Message::GetInstance().Get_c("MSG_DEBUG_SELECT"), selectColor);
	DrawString(0, 60, Message::GetInstance().Get_c("MSG_DEBUG_PLAYING"), playColor);
	DrawString(0, 80, Message::GetInstance().Get_c("MSG_DEBUG_OPTION"), optionColor);
}