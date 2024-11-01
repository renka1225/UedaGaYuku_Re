#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "UiMenu.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneOption.h"
#include "SceneTitle.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer) :
	m_pPrevScene(pScene)
{
	m_pUi = std::make_shared<UiMenu>(pPlayer);
	m_select = Select::kEnhance;
}

SceneMenu::~SceneMenu()
{
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
			//return std::make_shared<SceneOption>(shared_from_this());	// アイテム画面に遷移
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
	m_pUi->Draw(); // UIを表示する

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_MENU");
#endif
}
