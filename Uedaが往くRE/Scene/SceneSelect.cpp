#include "SceneSelect.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneSelect::SceneSelect()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneSelect::~SceneSelect()
{
}

/// <summary>
/// ������
/// </summary>
void SceneSelect::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneSelect::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_SELECT");
#endif
}

