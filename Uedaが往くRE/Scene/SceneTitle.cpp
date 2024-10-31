#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	/*if (!CheckSoundMem(Sound::GetInstance().GetBgmVol(Sound::BgmKind::kTitleBGM)))
	{
		Sound::GetInstance().PlayBGM(Sound::BgmKind::kTitleBGM);
	}*/

	if (input.IsTriggered(InputId::kOk))
	{
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

