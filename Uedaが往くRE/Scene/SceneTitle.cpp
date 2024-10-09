#include "Input.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	if (input.IsTriggered("OK"))
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

