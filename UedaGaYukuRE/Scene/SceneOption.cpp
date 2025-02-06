#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "EffectManager.h"
#include "Font.h"
#include "Sound.h"
#include "LoadCsv.h"
#include "UiBase.h"
#include "SceneDebug.h"
#include "SceneOption.h"
#include <algorithm>
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

	// 表示位置
	const std::map<std::string, Vec2> kDispPos =
	{
		{"text", { 92.0f, 78.0f }},			// テキスト表示位置
		{"bar", {800.0f, 380.0f}},			// 音量バー表示位置
		{"soundText", {750.0f, 300.0f}},	// サウンドのテキスト表示位置
		{"winText", {750.0f, 300.0f}},		// 画面サイズのテキスト表示位置
		{"nowWinText", {1200.0f, 370.0f}},	// 現在の画面状態のテキスト表示位置
		{"vol", { 1650.0f, 370.0f }},		// 現在の音量表示位置
		{"arrow", {1100.0f, 370.0f}}		// 矢印表示位置
	};
	constexpr float kTextInterval = 170.0f;		// テキスト表示間隔
	constexpr float kWindowModeTextAdj = 25.0f; // 画面サイズ表示位置調整

	const std::string kCursorId = "cursor_option";				// カーソルのID
	const std::string kSoundCursorId = "cursor_option_sound";	// サウンドカーソルのID
	constexpr float kCursorInterval = 150.0f;					// カーソルの表示間隔
	constexpr int kSoundBarColor = 0xcf2223;					// 現在の音量バーの色
}

SceneOption::SceneOption(std::shared_ptr<SceneBase> pScene):
	m_pPrevScene(pScene),
	m_afterSelect(SelectSound::kBGM),
	m_bgmVol(0),
	m_seVol(0),
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
	//選択状態更新
	if (m_isSound)
	{
		UpdateSound(input);	 // サウンド更新
		m_pUi->UpdateCursor(kSoundCursorId); // カーソル更新

		if (input.IsTriggered(InputId::kBack))
		{
			m_isSound = false;
		}
	}
	else if (m_isWindow)
	{
		UpdateWindowMode(input); // 画面サイズ更新
		m_pUi->UpdateCursor(kSoundCursorId); // カーソル更新

		if (input.IsTriggered(InputId::kBack))
		{
			m_isWindow = false;
		}
	}
	else
	{
		UpdateSelect(input, Select::kSelectNum);
		m_pUi->UpdateCursor(kCursorId); // カーソル更新

		if (input.IsTriggered(InputId::kBack))
		{
			SoundCancelSe();
			return m_pPrevScene;
		}

		if (input.IsTriggered(InputId::kOk))
		{
			if (m_select == Select::kSound)
			{
				m_isSound = true;
				m_afterSelect = SelectSound::kBGM;
			}
			else if (m_select == Select::kWindow)
			{
				m_isWindow = true;
				m_afterSelect = SelectWindow::kWindowMode;
			}
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
		m_pUi->DrawCursor(kSoundCursorId, m_afterSelect, kTextInterval);
	}
	else if (m_isWindow)
	{
		m_pUi->DrawCursor(kSoundCursorId, 0, kTextInterval);
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

	DrawGraphF(kDispPos.at("text").x, kDispPos.at("text").y, m_handle[Handle::kText], true);

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
		m_pUi->Init();
		//PlaySoundMem(Sound::GetInstance().[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_afterSelect = (m_afterSelect + (SelectSound::kSelectSoundNum - 1)) % SelectSound::kSelectSoundNum;
		m_pUi->Init();
		//PlaySoundMem(Sound::GetInstance().m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}

	// BGM選択中の場合
	if (m_afterSelect == SelectSound::kBGM)
	{
		Sound::GetInstance().ChangeBgmVol(input);
	}
	// SE選択中の場合
	else if (m_afterSelect == SelectSound::kSE)
	{
		Sound::GetInstance().ChangeSeVol(input);
	}
}

void SceneOption::UpdateWindowMode(Input& input)
{
	// 選択状態を1つ左にずらす
	if (input.IsTriggered(InputId::kLeft))
	{
		m_afterSelect = (m_afterSelect + 1) % SelectWindow::kSelectWinNum;
		EffectManager::GetInstance().Init();
	}
	// 選択状態を1つ右にずらす
	if (input.IsTriggered(InputId::kRight))
	{
		m_afterSelect = (m_afterSelect + (SelectWindow::kSelectWinNum - 1)) % SelectWindow::kSelectWinNum;
		EffectManager::GetInstance().Init();
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
	// 現在の音量を取得する
	int bgmVol = Sound::GetInstance().GetBgmVol();
	int seVol = Sound::GetInstance().GetSeVol();

	// サウンドバー表示
	DrawGraphF(kDispPos.at("bar").x, kDispPos.at("bar").y + kTextInterval * static_cast<int>(SelectSound::kBGM), m_handle[Handle::kSoundBar], true);
	DrawGraphF(kDispPos.at("bar").x, kDispPos.at("bar").y + kTextInterval * static_cast<int>(SelectSound::kSE), m_handle[Handle::kSoundBar], true);

	// 音量に合わせて四角の長さを更新する
	std::string barId = "bar_sound";
	auto bgData = LoadCsv::GetInstance().GetUiData(barId);

	float bgmBarWidth = bgData.width * (bgmVol / 100.0f);	// BGMバーの横幅
	float seBarWidth = bgData.width * (seVol / 100.0f);		// SEバーの横幅

	// BGM
	DrawBoxAA(bgData.LTposX, bgData.LTposY + kTextInterval * static_cast<int>(SelectSound::kBGM),
		bgData.LTposX + bgmBarWidth, bgData.RBposY + kTextInterval * static_cast<int>(SelectSound::kBGM),
		kSoundBarColor, true);

	// SE
	DrawBoxAA(bgData.LTposX, bgData.LTposY + kTextInterval * static_cast<int>(SelectSound::kSE),
		bgData.LTposX + seBarWidth, bgData.RBposY + kTextInterval * static_cast<int>(SelectSound::kSE),
		kSoundBarColor, true);

	// 音量によってつまみの位置を更新する
	//float bgmKnobPosX = kSoundKnobMinPosX + (kSoundKnobPos.x - kSoundKnobMinPosX) * (bgmVol / 100.0f);
	//float seKnobPosX = kSoundKnobMinPosX + (kSoundKnobPos.x - kSoundKnobMinPosX) * (seVol / 100.0f);
	//DrawGraphF(bgmKnobPosX, kSoundKnobPos.y + kSelectTextInterval * static_cast<int>(SelectSound::kBGM), m_handle[static_cast<int>(Handle::kSoundBarKnob)], true);
	//DrawGraphF(seKnobPosX, kSoundKnobPos.y + kSelectTextInterval * static_cast<int>(SelectSound::kSE), m_handle[static_cast<int>(Handle::kSoundBarKnob)], true);

	// テキスト表示
	DrawStringFToHandle(kDispPos.at("soundText").x, kDispPos.at("soundText").y + kTextInterval * static_cast<int>(SelectSound::kBGM),
		"BGM", Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_soundText)]);
	DrawStringFToHandle(kDispPos.at("soundText").x, kDispPos.at("soundText").y + kTextInterval * static_cast<int>(SelectSound::kSE),
		"SE", Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_soundText)]);

	// 音量表示
	DrawFormatStringFToHandle(kDispPos.at("vol").x, kDispPos.at("vol").y + kTextInterval * static_cast<int>(SelectSound::kBGM),
		Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_soundVol)], "%d", bgmVol);
	DrawFormatStringFToHandle(kDispPos.at("vol").x, kDispPos.at("vol").y + kTextInterval * static_cast<int>(SelectSound::kSE),
		Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_soundVol)], "%d", seVol);
}

void SceneOption::DrawWindowMode()
{
	//テキスト表示
	DrawStringFToHandle(kDispPos.at("winText").x, kDispPos.at("winText").y,
		"ウィンドウモード", Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_window)]);

	if (m_afterSelect == SelectWindow::kFullScreen)
	{
		DrawStringFToHandle(kDispPos.at("nowWinText").x, kDispPos.at("nowWinText").y,
			"フルスクリーン", Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_window)]);
	}
	else
	{
		// テキスト表示
		DrawStringFToHandle(kDispPos.at("nowWinText").x + kWindowModeTextAdj, kDispPos.at("nowWinText").y,
			"ウィンドウ", Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kOption_window)]);
	}

	// 矢印表示
	DrawGraphF(kDispPos.at("arrow").x, kDispPos.at("arrow").y, m_handle[static_cast<int>(Handle::kArrow)], true);
}