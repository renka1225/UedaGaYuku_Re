#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

namespace
{
	// シーンの種類
	enum SelectScene
	{
		kContinue,	// 続きから
		kFirst,		// 初めから
		kOption,	// オプション
		kGameEnd,	// ゲーム終了
		kSelectNum,	// 選択数
	};
}

SceneSelect::SceneSelect()
{
	//SaveData::GetInstance().Load(); // セーブデータを読み込む
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// 選択状態更新
	UpdateSelect(input, SelectScene::kSelectNum);

	// 遷移
	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == SelectScene::kContinue)
		{
			// 選択されたセーブデータを読み込む
			SaveData::GetInstance().Load();

			return std::make_shared<SceneMain>();
		}
		if (m_select == kFirst)
		{
			// 新しくセーブデータを作成する
			SaveData::GetInstance().CreateNewData();

			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == SelectScene::kGameEnd)
		{
			DxLib_End();
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
	int continueColor = Color::kColorW;
	int firstColor = Color::kColorW;
	int optionColor = Color::kColorW;
	int gameEndColor = Color::kColorW;

	if (m_select == SelectScene::kContinue) continueColor = Color::kColorR;
	if (m_select == SelectScene::kFirst) firstColor = Color::kColorR;
	if (m_select == SelectScene::kOption) optionColor = Color::kColorR;
	if (m_select == SelectScene::kGameEnd) gameEndColor = Color::kColorR;

	DrawString(0, 60, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_CONTINUE"), continueColor);
	DrawString(0, 80, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_FIRST"), firstColor);
	DrawString(0, 100, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_OPTION"), optionColor);
	DrawString(0, 120, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_GAMEEND"), gameEndColor);

	// セーブデータを表示する
	auto saveData = SaveData::GetInstance().GetSaveData();
	DrawFormatString(0, 180, Color::kColorW, "保存位置(X:%.2f,Y:%.2f,Z:%.2f)", saveData.playerPos.x, saveData.playerPos.y, saveData.playerPos.z);
	DrawFormatString(0, 200, Color::kColorW, "所持金 %d", saveData.money);

	DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif
}

