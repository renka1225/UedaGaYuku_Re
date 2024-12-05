#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

namespace
{
	// 画像の種類
	enum Handle
	{
		kBg,		// 背景
		kLogo,		// ロゴ
		kLogo_RE,	// ロゴのRE部分
		kLogo_back, // ロゴの後ろ部分
		kPRESS,		// PRESS ANY BUTTONのUI
		kNum		// 画像の種類
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/title/bg.png",
		"data/ui/title/logo.png",
		"data/ui/title/logo_RE.png",
		"data/ui/title/logo_back.png",
		"data/ui/title/press.png"
	};

	const char* kLogoDispSE = "logo.mp3";	// ロゴ表示SE名
	const char* kBgm = "title.mp3";			// BGM名
	constexpr int kBGMTime = 150;			// BGMを再生しはじめる時間

	/*タイトルロゴ*/
	const Vec2 kTitleLogoPos = { 900.0f, 400.0f };		// タイトルロゴ表示位置
	const Vec2 kTitleLogo_REPos = { 1190.0f, 360.0f };	// タイトルロゴ(RE)表示位置
	constexpr float kTitleLogoMinScale = 0.8f;			// タイトルロゴの最小サイズ
	constexpr float kTitleLogoMaxScale = 12.0f;			// タイトルロゴの最大サイズ
	constexpr float kTitleLogoChangeScale = 0.5f;		// タイトルロゴのサイズ変化量
	constexpr float kTitleLogoInitRot = 360.0f;			// 開始時のタイトルロゴの回転率
	constexpr float kTitleLogoChangeRot = 20.0f;		// タイトルロゴの回転率変化量
	constexpr int kTitleTime = 60;						// タイトルを表示するまでの時間
	constexpr int kTitle_RETime = 80;					// タイトル(RE)を表示するまでの時間
	constexpr int kPressTime = 150;						// PRESSを表示するまでの時間

	/*テキスト*/
	const Vec2 kTextPos = { 500.0f, 850.0f };			// "PRESS ANY BUTTON"のテキスト位置
	constexpr int kTextDisplayTime = 2;					// テキストを表示する間隔
	constexpr int kTextDisplayAnimTime = 240;			// テキストアニメーションの時間
	constexpr int kMaxAlpha = 255;						// 最大アルファ値
	constexpr int kMinAlpha = 30;						// 最小アルファ値
	constexpr int kTextTime = 120;						// テキストを表示するまでの時間
	constexpr int kTextColor = 0x000000;				// テキストの色

}

SceneTitle::SceneTitle():
	m_time(0),
	m_textDisplayTime(0),
	m_titleLogoScale(kTitleLogoMaxScale),
	m_titleLogoRot(kTitleLogoInitRot),
	m_textAlpha(kMinAlpha)
{
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneTitle::~SceneTitle()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void SceneTitle::Init()
{
	m_titleLogoScale = kTitleLogoMaxScale;
	m_titleLogoRot = kTitleLogoInitRot;
}

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	m_time++; // 経過時間を更新
	UpdatePressText();
	UpdateLogo();
	UpdateSound();

	if (m_time < kPressTime) return shared_from_this(); // 演出中は操作できないようにする

	// シーン遷移する
	if (CheckInputSceneChange(input))
	{
		Sound::GetInstance().StopBgm(kBgm);
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneTitle::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kBg], true);

	// タイトルロゴ表示
	DrawRectRotaGraphF(kTitleLogoPos.x, kTitleLogoPos.y, 0, 0, Game::kScreenWidth, Game::kScreenHeight, m_titleLogoScale, m_titleLogoRot, m_handle[Handle::kLogo_back], true);

	if (m_time > kTitle_RETime)
	{
		DrawRectRotaGraphF(kTitleLogo_REPos.x, kTitleLogo_REPos.y, 0, 0, Game::kScreenWidth, Game::kScreenHeight, m_titleLogoScale, 0.0f, m_handle[Handle::kLogo_RE], true);
	}
	if (m_time > kTitleTime)
	{
		DrawRectRotaGraphF(kTitleLogoPos.x, kTitleLogoPos.y, 0, 0, Game::kScreenWidth, Game::kScreenHeight, m_titleLogoScale, 0.0f, m_handle[Handle::kLogo], true);
	}

	// "PRESSANYBUTTON"表示
	if (m_time > kPressTime)
	{
		// 文字を点滅させる
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);
		DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kPRESS], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_TITLE");
#endif
}

void SceneTitle::UpdatePressText()
{
	// テキストのアルファ値を調整する
	m_textDisplayTime += kTextDisplayTime;
	m_textDisplayTime %= kTextDisplayAnimTime;
	// MEMO:sin波を使って0～1の範囲にする
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_textDisplayTime) / kTextDisplayAnimTime * DX_PI_F);
	m_textAlpha = kMinAlpha + static_cast<int>((kMaxAlpha - kMinAlpha) * sinAlpha);
}

void SceneTitle::UpdateLogo()
{
	// タイトルロゴのサイズをだんだん小さくする
	m_titleLogoScale -= kTitleLogoChangeScale;
	m_titleLogoScale = std::max(kTitleLogoMinScale, m_titleLogoScale);

	// タイトルロゴを回転させる
	m_titleLogoRot -= kTitleLogoChangeRot;
	m_titleLogoRot = std::max(0.0f, m_titleLogoRot);
}

void SceneTitle::UpdateSound()
{
	// タイトルロゴ表示後BGMを鳴らす
	if (m_time < kBGMTime)
	{
		if (!Sound::GetInstance().GetIsPlaySe(kLogoDispSE))
		{
			Sound::GetInstance().PlaySe(kLogoDispSE);
		}
	}
	else
	{
		if (!Sound::GetInstance().GetIsPlayBgm(kBgm))
		{
			Sound::GetInstance().PlayBgm(kBgm);
		}
	}
}

bool SceneTitle::CheckInputSceneChange(Input& input)
{
	if(input.IsTriggered(InputId::kA)) return true;
	if(input.IsTriggered(InputId::kB)) return true;
	if(input.IsTriggered(InputId::kX)) return true;
	if(input.IsTriggered(InputId::kY)) return true;
	if(input.IsTriggered(InputId::kRb)) return true;
	if(input.IsTriggered(InputId::kLb)) return true;
	if(input.IsTriggered(InputId::kUp)) return true;
	if(input.IsTriggered(InputId::kDown)) return true;
	if(input.IsTriggered(InputId::kLeft)) return true;
	if(input.IsTriggered(InputId::kRight)) return true;

	return false;
}

