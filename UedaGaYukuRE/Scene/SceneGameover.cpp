#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
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
		kBg,
		kGameover,
		kText,
		kNum
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/gameover/bg.png",
		"data/ui/gameover/gameover.png",
		"data/ui/gameover/text.png"
	};

	const Vec2 kClearPos = { 470.0f, 129.0f }; // ゲームオーバー表示位置
	const Vec2 kTextPos = { 743.0f, 633.0f };  // テキスト表示位置
	constexpr int kBgColor = 0x1c1c1c;		   // 背景色
	const std::string kCursorId = "cursor_gameover"; // カーソルのID
	constexpr float kCursorInterval = 195.0f;		 // カーソルの表示間隔

	/*フェード*/
	constexpr int kStartFadeAlpha = 255; // スタート時のフェード値
	constexpr int kFadeFrame = 8;		 // フェード変化量
}

SceneGameover::SceneGameover()
{
	Sound::GetInstance().PlayBgm(SoundName::kBgm_gameover);

	m_select = Select::kMainScene;
	m_fadeAlpha = kStartFadeAlpha;

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
	FadeOut(kFadeFrame); // フェードアウト
	if (m_isFadeOut) return shared_from_this(); // フェードアウト中は操作できないようにする

	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kMainScene)
		{
			SceneChangeSound(SoundName::kBgm_gameover);
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneMain>();
		}
		else if (m_select == Select::kTitle)
		{
			SceneChangeSound(SoundName::kBgm_gameover);
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneTitle>();
		}
	}

	return shared_from_this();
}

void SceneGameover::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kBg], true);
	DrawGraphF(kClearPos.x, kClearPos.y, m_handle[Handle::kGameover], true);
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);
	DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kText], true);

	DrawFade();	// フェードインアウト描画

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_GAMEOVER");
#endif
}
