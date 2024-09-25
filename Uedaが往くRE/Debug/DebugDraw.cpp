#include "Game.h"
#include "DebugDraw.h"

/// <summary>
/// コンストラクタ
/// </summary>
DebugDraw::DebugDraw()
{
}

/// <summary>
/// デストラクタ
/// </summary>
DebugDraw::~DebugDraw()
{
}

/// <summary>
/// 初期化
/// </summary>
void DebugDraw::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void DebugDraw::Update()
{
}

/// <summary>
/// デバッグ情報描画
/// </summary>
/// <param name="pos">座標</param>
void DebugDraw::DrawInfo(const VECTOR pos)
{
	DrawFormatString(0, 0, TextColor::kColorW, "座標(X:%f, Y:%f, Z:%f)", pos.x, pos.y, pos.z);
}
