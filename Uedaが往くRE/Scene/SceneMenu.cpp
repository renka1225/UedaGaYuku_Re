#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneUseItem.h"
#include "SceneEnhance.h"
#include "SceneOption.h"
#include "SceneSave.h"
#include "SceneTitle.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPrevScene = pScene;
	m_pPlayer = pPlayer;
	m_select = Select::kItem;

	m_bgHandle = LoadGraph("data/ui/bg_menu.png");
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
			return std::make_shared<SceneUseItem>(shared_from_this(), m_pPlayer);	// アイテム画面に遷移
		}
		else if (m_select == Select::kEnhance)
		{
			return std::make_shared<SceneEnhance>(shared_from_this(), m_pPlayer);	// 強化画面に遷移
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());	// オプション画面に遷移
		}
		else if (m_select == Select::kSave)
		{
			return std::make_shared<SceneSave>(shared_from_this(), m_pPlayer); // セーブ画面に遷移
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
	m_pPrevScene->Draw();				// ゲーム中の画面を表示する
	DrawGraph(0, 0, m_bgHandle, true);  // 背景表示
	DrawMoney(m_pPlayer);				// 所持金額表示

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_MENU");
#endif
}
