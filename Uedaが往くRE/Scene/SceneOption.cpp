#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "UiBase.h"
#include "SceneDebug.h"
#include "SceneOption.h"
#include <cassert>

namespace
{
	// 選択肢
	enum Select
	{
		kSound,		// サウンド
		kWindow,	// 画面サイズ
		kSelectNum	// 選択数
	};

	// サウンド選択肢
	enum SelectSound
	{
		kBGM,			// BGM
		kSE,			// SE
		kSelectSoundNum	// 選択数
	};

	// ウィンドウモード選択肢
	enum SelectWindow
	{
		kFullScreen,	// フルスクリーン
		kWindowMode,	// ウィンドウモード
		kSelectWinNum	// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kBg,			// 背景
		kText,			// テキスト
		kSoundBar,		// サウンドバー
		kSoundBarKnob,	// サウンドバーのつまみ
		kArrow,			// 矢印
		kHandleNum		// 画像の種類
	};

	const char* kHandlePath[Handle::kHandleNum]
	{
		"data/ui/option/bg.png",
		"data/ui/option/top.png",
		"data/ui/option/soundBar.png",
		"data/ui/option/soundKnob.png",
		"data/ui/option/arrow.png",
	};

	const Vec2 kTextPos = { 92.0f, 78.0f }; // テキスト表示位置

	const std::string kCursorId = "cursor_option";				// カーソルのID
	const std::string kSoundCursorId = "cursor_option_sound";	// サウンドカーソルのID
	constexpr float kCursorInterval = 150.0f;	 // カーソルの表示間隔
}

SceneOption::SceneOption(std::shared_ptr<SceneBase> pScene):
	m_pPrevScene(pScene),
	m_afterSelect(SelectSound::kBGM),
	m_isSound(false),
	m_isWindow(false)
{
	// 画像読み込み
	m_handle.resize(Handle::kHandleNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
		assert(m_handle[i] != -1);
	}
}

SceneOption::~SceneOption()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	m_pUi->UpdateCursor(kCursorId);

	//選択状態更新
	if (m_isSound)
	{
		UpdateSound(input);		 // サウンド更新
	}
	else if (m_isWindow)
	{
		UpdateWindowMode(input); // 画面サイズ更新
	}
	else
	{
		UpdateSelect(input, Select::kSelectNum);
	}

	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene;
	}
	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kSound)
		{
			m_isSound = true;

			// SE選択中にOKボタンを押しても状態が変わらないようにする
			if (m_afterSelect == SelectSound::kSE)
			{
				m_afterSelect = SelectSound::kSE;
			}
			else
			{
				m_afterSelect = SelectSound::kBGM;
			}

		}
		else if (m_select == Select::kWindow)
		{
			m_isWindow = true;
			m_afterSelect = SelectWindow::kFullScreen;
		}
	}

	return shared_from_this();
}

void SceneOption::Draw()
{
	// 背景表示
	DrawGraph(0, 0, m_handle[Handle::kBg], true);

	// カーソル表示
	if (m_isSound)
	{
		m_pUi->DrawCursor(kSoundCursorId, m_select, kCursorInterval);
	}
	else if (m_isWindow)
	{
		m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);
	}
	else
	{
		m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);
	}

	// サウンド関連表示
	if (m_select == Select::kSound)
	{
		DrawSound();
	}
	// 画面サイズ関連表示
	else if (m_select == Select::kWindow)
	{
		DrawWindowMode();
	}

	DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kText], true);

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}

void SceneOption::UpdateSound(Input& input)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered(InputId::kDown))
	{
		m_afterSelect = (m_afterSelect + 1) % SelectSound::kSelectSoundNum;
		//m_pUI->Init();
		//PlaySoundMem(Sound::GetInstance().[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_afterSelect = (m_afterSelect + (SelectSound::kSelectSoundNum - 1)) % SelectSound::kSelectSoundNum;
		//m_pUI->Init();
		//PlaySoundMem(Sound::GetInstance().m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}

	// BGM選択中の場合
	if (m_afterSelect == SelectSound::kBGM)
	{
		//Sound::GetInstance().ChangeBgmVol(input);
	}
	// SE選択中の場合
	if (m_afterSelect == SelectSound::kSE)
	{
		//Sound::GetInstance().ChangeSeVol(input);
	}
}

void SceneOption::UpdateWindowMode(Input& input)
{
	// 選択状態を1つ左にずらす
	if (input.IsTriggered(InputId::kLeft))
	{
		m_afterSelect = (m_afterSelect + 1) % SelectWindow::kSelectWinNum;
		//PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
	// 選択状態を1つ右にずらす
	if (input.IsTriggered(InputId::kRight))
	{
		m_afterSelect = (m_afterSelect + (SelectWindow::kSelectWinNum - 1)) % SelectWindow::kSelectWinNum;
		//PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}

	// 画面サイズを変更する
	if (m_afterSelect == SelectWindow::kWindowMode)
	{
		ChangeWindowMode(true);		// ウィンドウモード
	}
	else
	{
		ChangeWindowMode(false);	// フルスクリーン
	}
}

void SceneOption::DrawSound()
{
	//// サウンドバー表示
	//DrawGraphF(kSoundBarPos.x, kSoundBarPos.y + kSelectTextInterval * SelectSound::kBGM, m_handle[static_cast<int>(Handle::kSoundBar)], true);
	//DrawGraphF(kSoundBarPos.x, kSoundBarPos.y + kSelectTextInterval * SelectSound::kSE, m_handle[static_cast<int>(Handle::kSoundBar)], true);

	//// 音量に合わせて四角の長さを更新する
	//float bgmBarWidth = kCurrentSoundBarWidth * (Sound::GetBgmVol() / 100.0f);
	//float seBarWidth = kCurrentSoundBarWidth * (Sound::GetSeVol() / 100.0f);
	//DrawBoxAA(kCurrentSoundBarPos.x, kCurrentSoundBarPos.y + kSelectTextInterval * SelectSound::kBGM,
	//	kCurrentSoundBarPos.x + bgmBarWidth, kCurrentSoundBarPos.y + kCurrentSoundBarHeight + kSelectTextInterval * SelectSound::kBGM,
	//	kCurrentSoundBarColor, true);
	//DrawBoxAA(kCurrentSoundBarPos.x, kCurrentSoundBarPos.y + kSelectTextInterval * SelectSound::kSE,
	//	kCurrentSoundBarPos.x + seBarWidth, kCurrentSoundBarPos.y + kCurrentSoundBarHeight + kSelectTextInterval * SelectSound::kSE,
	//	kCurrentSoundBarColor, true);

	//// 音量によってつまみの位置を更新する
	//float bgmKnobPosX = kSoundKnobMinPosX + (kSoundKnobPos.x - kSoundKnobMinPosX) * (Sound::GetBgmVol() / 100.0f);
	//float seKnobPosX = kSoundKnobMinPosX + (kSoundKnobPos.x - kSoundKnobMinPosX) * (Sound::GetSeVol() / 100.0f);
	//DrawGraphF(bgmKnobPosX, kSoundKnobPos.y + kSelectTextInterval * SelectSound::kBGM, m_handle[static_cast<int>(Handle::kSoundKnob)], true);
	//DrawGraphF(seKnobPosX, kSoundKnobPos.y + kSelectTextInterval * SelectSound::kSE, m_handle[static_cast<int>(Handle::kSoundKnob)], true);

	//// テキスト表示
	//DrawStringFToHandle(kAfterSelectTextPos.x, kAfterSelectTextPos.y + kSelectTextInterval * SelectSound::kBGM,
	//	"BGM", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)]);
	//DrawStringFToHandle(kAfterSelectTextPos.x, kAfterSelectTextPos.y + kSelectTextInterval * SelectSound::kSE,
	//	"SE", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)]);
	//DrawFormatStringFToHandle(kSoundNumTextPos.x, kSoundNumTextPos.y + kSelectTextInterval * SelectSound::kBGM,
	//	kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)], "%d", Sound::GetBgmVol());
	//DrawFormatStringFToHandle(kSoundNumTextPos.x, kSoundNumTextPos.y + kSelectTextInterval * SelectSound::kSE,
	//	kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)], "%d", Sound::GetSeVol());
}

void SceneOption::DrawWindowMode()
{
	// テキスト表示
	//DrawStringFToHandle(kAfterSelectTextPos.x, kAfterSelectTextPos.y,
	//	"ウィンドウモード", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)]);

	//// 矢印表示
	//DrawGraphF(kArrowPos.x, kArrowPos.y, m_handle[static_cast<int>(Handle::kArrow)], true);

	//if (m_afterSelect == SelectWindow::kFullScreen)
	//{
	//	DrawStringFToHandle(kWindowModeTextPos.x, kWindowModeTextPos.y,
	//		"フルスクリーン", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)]);
	//}
	//else
	//{
	//	// テキスト表示
	//	DrawStringFToHandle(kWindowModeTextPos.x + kWindowModeTextAdj, kWindowModeTextPos.y,
	//		"ウィンドウ", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption)]);
	//}
}
