#include "Vec2.h"
#include "Input.h"
#include "SceneMain.h"
#include "SceneGameover.h"

SceneGameover::SceneGameover()
{
}

SceneGameover::~SceneGameover()
{
}

void SceneGameover::Init()
{
}

std::shared_ptr<SceneBase> SceneGameover::Update(Input& input)
{
	if (input.IsTriggered(InputId::kOk))
	{
		return std::make_shared<SceneMain>();
	}

	return shared_from_this();
}

void SceneGameover::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_GAMEOVER");
#endif
}
