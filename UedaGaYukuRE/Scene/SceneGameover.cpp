#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "UiBase.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneGameover.h"

namespace
{
	// 選択項目
	enum Select
	{
		kMainScene, // ゲームに戻る
		kTitle,		// タイトルに戻る
		kSelectNum	// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kGameover,
		kText,
		kNum
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/gameover/gameover.png",
		"data/ui/gameover/text.png"
	};

	const Vec2 kClearPos = { 400.0f, 100.0f }; // ゲームオーバー表示位置
	const Vec2 kTextPos = { 743.0f, 633.0f };  // テキスト表示位置
	constexpr int kBgColor = 0x1c1c1c; // 背景色
	const std::string kCursorId = "cursor_gameover"; // カーソルのID
	constexpr float kCursorInterval = 195.0f;		 // カーソルの表示間隔
}

SceneGameover::SceneGameover()
{
	m_select = Select::kMainScene;

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneGameover::~SceneGameover()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneGameover::Update(Input& input)
{
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kMainScene)
		{
			return std::make_shared<SceneMain>();
		}
		else if (m_select == Select::kTitle)
		{
			return std::make_shared<SceneTitle>();
		}
	}

	return shared_from_this();
}

void SceneGameover::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBgColor, true);
	DrawGraphF(kClearPos.x, kClearPos.y, m_handle[Handle::kGameover], true);
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);
	DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kText], true);

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_GAMEOVER");
#endif
}
