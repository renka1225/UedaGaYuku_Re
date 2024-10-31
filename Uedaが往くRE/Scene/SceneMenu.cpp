#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "SceneDebug.h"
#include "SceneOption.h"
#include "SceneTitle.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu(std::shared_ptr<SceneBase> pScene) :
	m_pPrevScene(pScene)
{
	m_select = Select::kEnhance;
	m_bgHandle = LoadGraph("data/ui/menuImage.png");
}

SceneMenu::~SceneMenu()
{
	DeleteGraph(m_bgHandle);
}

void SceneMenu::Init()
{
}

std::shared_ptr<SceneBase> SceneMenu::Update(Input& input)
{
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);

	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // ゲーム画面に戻る
	}
	else if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kItem)
		{
			//return std::make_shared<SceneOption>(shared_from_this());	// 強化画面に遷移
		}
		else if (m_select == Select::kEnhance)
		{
			//return std::make_shared<SceneOption>(shared_from_this());	// 強化画面に遷移
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());	// オプション画面に遷移
		}
		else if (m_select == Select::kSave)
		{
			//return std::make_shared<SceneSelectStage>(); // セーブ画面に遷移
		}
		else if (m_select == Select::kBackTitle)
		{
			return std::make_shared<SceneTitle>(); // タイトル画面に遷移
		}
	}
	else if (input.IsTriggered("pause"))
	{
		return m_pPrevScene;	// ゲーム画面にもどる
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneMenu::Draw()
{
	DrawGraph(0, 0, m_bgHandle, true);

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_MENU");
#endif
}
