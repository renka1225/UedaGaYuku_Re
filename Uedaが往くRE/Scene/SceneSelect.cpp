#include "SceneSelect.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneSelect::SceneSelect()
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneSelect::~SceneSelect()
{
}

/// <summary>
/// 初期化
/// </summary>
void SceneSelect::Init()
{
}

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneSelect::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_SELECT");
#endif
}

