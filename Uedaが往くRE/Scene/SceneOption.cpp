#include "SceneOption.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneOption::SceneOption()
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneOption::~SceneOption()
{
}

/// <summary>
/// 初期化
/// </summary>
void SceneOption::Init()
{
}

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_OPTION");
#endif
}
