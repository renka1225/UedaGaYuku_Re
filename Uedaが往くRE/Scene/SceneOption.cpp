#include "Game.h"
#include "Input.h"
#include "SceneDebug.h"
#include "SceneOption.h"

SceneOption::SceneOption(std::shared_ptr<SceneBase> pScene):
	m_pPrevScene(pScene)
{
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	UpdateSelect(input, Select::kSelectNum);

	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene;
	}

	return shared_from_this();
}

void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}
