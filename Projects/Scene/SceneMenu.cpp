#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "UiBase.h"
#include "Camera.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneUseItem.h"
#include "SceneEnhance.h"
#include "SceneOption.h"
#include "SceneSave.h"
#include "SceneTitle.h"
#include "SceneMenu.h"

namespace
{
	// 選択項目
	enum Select
	{
		kItem,			// アイテム使用
		kEnhance,		// 強化
		kOption,		// オプション
		kSave,			// セーブする
		kBackTitle,		// タイトルに戻る
		kSelectNum		// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kMenuBg,	// 背景
		kMenuText,	// テキスト
		kNum		// ハンドルの数
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/menu/bg.png",
		"data/ui/menu/text.png"
	};

	const std::string kCursorId = "cursor_menu"; // カーソルのID
	constexpr float kCursorInterval = 136.0f;	 // カーソルの表示間隔
}

SceneMenu::SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera)
{
	m_pPrevScene = pScene;
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_select = Select::kItem;

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneMenu::~SceneMenu()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void SceneMenu::Init()
{
}

std::shared_ptr<SceneBase> SceneMenu::Update(Input& input)
{
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	if (input.IsTriggered(InputId::kBack))
	{
		SoundCancelSe();
		return m_pPrevScene; // ゲーム画面に戻る
	}
	else if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kItem)
		{
			return std::make_shared<SceneUseItem>(shared_from_this(), m_pPlayer); // アイテム画面に遷移
		}
		else if (m_select == Select::kEnhance)
		{
			return std::make_shared<SceneEnhance>(shared_from_this(), m_pPlayer); // 強化画面に遷移
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this()); // オプション画面に遷移
		}
		else if (m_select == Select::kSave)
		{
			return std::make_shared<SceneSave>(shared_from_this(), m_pPlayer, m_pCamera); // セーブ画面に遷移
		}
		else if (m_select == Select::kBackTitle)
		{
			return std::make_shared<SceneTitle>(); // タイトル画面に遷移
		}
	}
	else if (input.IsTriggered(InputId::kPause))
	{
		return m_pPrevScene;	// ゲーム画面にもどる
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneMenu::Draw()
{
	m_pPrevScene->Draw(); // ゲーム中の画面を表示する
	
	DrawGraph(0, 0, m_handle[Handle::kMenuBg], true);			// 背景表示
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);	// カーソルの表示
	DrawGraph(0, 0, m_handle[Handle::kMenuText], true);			// テキスト表示

	DrawMoney(m_pPlayer);	// 所持金額表示

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_MENU");
#endif
}
