#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

/// <summary>
/// ������
/// </summary>
void SceneTitle::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_TITLE");
#endif
}

