#include "Game.h"
#include "Input.h"
#include "SceneDebug.h"
#include "SceneOption.h"

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	if (input.IsTriggered(InputId::kBack))
	{
		return std::make_shared<SceneDebug>();
	}

	return shared_from_this();
}

void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}
