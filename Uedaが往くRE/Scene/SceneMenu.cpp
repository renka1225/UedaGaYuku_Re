#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneOption.h"
#include "SceneTitle.h"
#include "SceneMenu.h"

namespace
{
	const Vec2 kDispMoneyBgPos = { 1320.0f, 15.0f };	// 金額の背景表示位置
	const Vec2 kDispMoneyTextPos = { 1700.0f, 80.0f };  // 所持金額表示位置
}

SceneMenu::SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer) :
	m_pPrevScene(pScene)
{
	m_pPlayer = pPlayer;
	m_select = Select::kItem;

	m_bgHandle = LoadGraph("data/ui/menu_bg.png");
	m_moneyBg = LoadGraph("data/ui/money_bg.png");
}

SceneMenu::~SceneMenu()
{
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_moneyBg);
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
			//return std::make_shared<SceneUseItem>(shared_from_this());	// アイテム画面に遷移
		}
		else if (m_select == Select::kEnhance)
		{
			//return std::make_shared<SceneEnhace>(shared_from_this());	// 強化画面に遷移
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());	// オプション画面に遷移
		}
		else if (m_select == Select::kSave)
		{
			//return std::make_shared<SceneSave>(); // セーブ画面に遷移
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
	DrawGraph(0, 0, m_bgHandle, true); // 背景表示
	DrawMoney(); // 所持金額表示

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_MENU");
#endif
}

void SceneMenu::DrawMoney()
{
	DrawGraphF(kDispMoneyBgPos.x, kDispMoneyBgPos.y, m_moneyBg, true);

	// 現在の所持金額
	DrawFormatStringF(kDispMoneyTextPos.x, kDispMoneyTextPos.y, Color::kColorW, "%d", m_pPlayer->GetMoney());
}
