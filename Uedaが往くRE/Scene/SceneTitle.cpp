#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

namespace
{
	const char* kBgm = "title.mp3";	// BGM名
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
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_TITLE");
#endif
}

