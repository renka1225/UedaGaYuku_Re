#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneDebug.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneDebug::SceneDebug()
{
	m_select = SelectScene::kMain;
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
	if (input.IsTriggered(InputId::kOk))
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
			return std::make_shared<SceneOption>(shared_from_this());
		}
	}

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneDebug::Draw()
{
	int titleColor = Color::kColorW;
	int selectColor = Color::kColorW;
	int playColor = Color::kColorW;
	int optionColor = Color::kColorW;

	if (m_select == SelectScene::kTitle) titleColor = Color::kColorR;
	if (m_select == SelectScene::kSelect) selectColor = Color::kColorR;
	if (m_select == SelectScene::kMain) playColor = Color::kColorR;
	if (m_select == SelectScene::kOption) optionColor = Color::kColorR;

	DrawString(0, 20, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_TITLE"), titleColor);
	DrawString(0, 40, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_SELECT"), selectColor);
	DrawString(0, 60, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_PLAYING"), playColor);
	DrawString(0, 80, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_OPTION"), optionColor);
}