#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "UiBase.h"
#include "SceneTitle.h"
#include "SceneClear.h"

namespace
{
	// 選択項目
	enum Select
	{
		kTitle,		// タイトルに戻る
		kGameEnd,	// ゲーム終了
		kSelectNum	// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kClear,
		kText,
		kNum
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/clear/clear.png",
		"data/ui/clear/text.png",
	};

	const Vec2 kGameoverPos = { 560.0f, 100.0f }; // クリア表示位置
	const Vec2 kTextPos = { 743.0f, 633.0f }; // テキスト表示位置
	constexpr int kBgColor = 0x9a9a9a; // 背景色
	const std::string kCursorId = "cursor_clear"; // カーソルのID
	constexpr float kCursorInterval = 195.0f;	  // カーソルの表示間隔

	/*フェード*/
	constexpr int kStartFadeAlpha = 255; // スタート時のフェード値
	constexpr int kFadeFrame = 8;		 // フェード変化量
}

SceneClear::SceneClear()
{
	m_fadeAlpha = kStartFadeAlpha;

	m_select = Select::kTitle;

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneClear::~SceneClear()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneClear::Update(Input& input)
{
	FadeOut(kFadeFrame); // フェードアウト
	if (m_isFadeOut) return shared_from_this(); // フェードアウト中は操作できないようにする

	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kTitle)
		{
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneTitle>();
		}
		else if (m_select == Select::kGameEnd)
		{
			EndGame();
		}
	}

	return shared_from_this();
}

void SceneClear::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBgColor, true);
	DrawGraphF(kGameoverPos.x, kGameoverPos.y, m_handle[Handle::kClear], true);
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);
	DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kText], true);

	DrawFade();	// フェードインアウト描画

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_CLEAR");
#endif
}