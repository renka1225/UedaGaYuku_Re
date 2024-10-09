#include "SceneOption.h"

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	return shared_from_this();
}

void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}
