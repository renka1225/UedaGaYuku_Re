#include "Input.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

/// <summary>
/// ‰Šú‰»
/// </summary>
void SceneTitle::Init()
{
}

/// <summary>
/// XV
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
/// •`‰æ
/// </summary>
void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_TITLE");
#endif
}

