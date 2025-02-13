﻿#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneDebug.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneDebug::SceneDebug()
{
	m_select = SelectScene::kMain;
}

SceneDebug::~SceneDebug()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
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
	UpdateSaveSelect(input, SaveData::SelectSaveData::kSaveNum);

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
			// 選択されたセーブデータを読み込む
			SaveData::GetInstance().Load(m_saveSelect);
			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == SelectScene::kClear)
		{
			return std::make_shared<SceneClear>();
		}
		else if (m_select == SelectScene::kGameover)
		{
			return std::make_shared<SceneGameover>();
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
	int clearColor = Color::kColorW;
	int gameoverColor = Color::kColorW;

	if (m_select == SelectScene::kTitle) titleColor = Color::kColorR;
	if (m_select == SelectScene::kSelect) selectColor = Color::kColorR;
	if (m_select == SelectScene::kMain) playColor = Color::kColorR;
	if (m_select == SelectScene::kOption) optionColor = Color::kColorR;
	if (m_select == SelectScene::kClear) clearColor = Color::kColorR;
	if (m_select == SelectScene::kGameover) gameoverColor = Color::kColorR;

	DrawString(0, 20, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_TITLE"), titleColor);
	DrawString(0, 40, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_SELECT"), selectColor);
	DrawString(0, 60, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_PLAYING"), playColor);
	DrawString(0, 80, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_OPTION"), optionColor);
	DrawString(0, 100, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_CLEAR"), clearColor);
	DrawString(0, 120, LoadCsv::GetInstance().Get_cMessage("MSG_DEBUG_GAMEOVER"), gameoverColor);
}