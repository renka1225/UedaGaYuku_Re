#include "DxLib.h"
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
		kBg,	// 背景
		kLogo,	// ロゴ
		kPRESS,	// PRESS ANY BUTTONのUI
		kNum	// 画像の種類
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/title/bg.png",
		"data/ui/title/logo.png",
		"data/ui/title/press.png"
	};

	const char* kBgm = "title.mp3";	// BGM名
}

SceneTitle::SceneTitle()
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

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	if (!Sound::GetInstance().GetIsPlayBgm(kBgm))
	{
		Sound::GetInstance().PlayBgm(kBgm);
	}

	if (input.IsTriggered(InputId::kOk))
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

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_TITLE");
#endif
}

