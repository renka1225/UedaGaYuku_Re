#include "SceneOption.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneOption::SceneOption()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneOption::~SceneOption()
{
}

/// <summary>
/// ������
/// </summary>
void SceneOption::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}
