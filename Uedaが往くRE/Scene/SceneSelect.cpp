#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// 選択状態更新
	UpdateSelect(input, SelectScene::kSelectNum);

	// 遷移
	if (input.IsTriggered(InputId::kOk))
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
	else if (input.IsTriggered(InputId::kBack))
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneSelect::Draw()
{
#ifdef _DEBUG
	int mainColor = Color::kColorW;
	int optionColor = Color::kColorW;

	if (m_select == SelectScene::kMain) mainColor = Color::kColorR;
	if (m_select == SelectScene::kOption) optionColor = Color::kColorR;

	DrawString(0, 60, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_PLAYING"), mainColor);
	DrawString(0, 80, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_OPTION"), optionColor);

	DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif
}

