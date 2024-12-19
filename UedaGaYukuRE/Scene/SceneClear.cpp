#include "Vec2.h"
#include "Input.h"
#include "SceneTitle.h"
#include "SceneClear.h"

SceneClear::SceneClear()
{
}

SceneClear::~SceneClear()
{
}

void SceneClear::Init()
{
}

std::shared_ptr<SceneBase> SceneClear::Update(Input& input)
{
	if (input.IsTriggered(InputId::kOk))
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneClear::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_CLEAR");
#endif
}