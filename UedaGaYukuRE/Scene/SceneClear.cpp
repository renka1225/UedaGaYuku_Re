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
		kEndSelect,
		kNum
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/clear/clear.png",
		"data/ui/clear/text.png",
		"data/ui/select/endSelect.png"
	};

	const Vec2 kGameoverPos = { 560.0f, 100.0f }; // クリア表示位置
	const Vec2 kTextPos = { 743.0f, 633.0f }; // テキスト表示位置
	constexpr int kBgColor = 0x9a9a9a; // 背景色

	/*カーソル*/
	const std::string kCursorId = "cursor_clear"; // カーソルのID
	constexpr float kCursorInterval = 195.0f;	  // カーソルの表示間隔

	const std::string kEndSelectCursorId = "cursor_endSelect";	// ゲーム終了選択肢カーソルのID
	constexpr float kEndSelectCursorInterval = 157.0f;			// ゲーム終了選択肢カーソル表示間隔
	const std::string kEndSelectBgId = "bg_endSelect";		// ゲーム終了選択肢の背景ID
	const Vec2 kDispEndSelectPos = { 528.0f, 279.0f };		// ゲーム終了選択肢の表示位置

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

	// 終了選択肢
	if (m_isChoice)
	{
		// 選択状態を更新
		UpdateChoice(input);
		m_pUi->UpdateCursor(kEndSelectCursorId);

		if (input.IsTriggered(InputId::kOk))
		{
			if (m_choiceSelect == Choice::kYes)
			{
				EndGame();
			}
			else if (m_choiceSelect == Choice::kNo)
			{
				// 選択肢を閉じる
				m_isChoice = false;
				m_choiceSelect = Choice::kYes;
				return shared_from_this();
			}
		}
	}

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
			m_isChoice = true;
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

	// ゲーム終了の選択肢を表示
	if (m_isChoice)
	{
		m_pUi->DrawChoiceBg(kEndSelectBgId); // 背景表示
		m_pUi->DrawCursor(kEndSelectCursorId, m_choiceSelect, kEndSelectCursorInterval); // カーソル表示
		DrawGraphF(kDispEndSelectPos.x, kDispEndSelectPos.y, m_handle[Handle::kEndSelect], true); // テキスト表示
	}

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_CLEAR");
#endif
}