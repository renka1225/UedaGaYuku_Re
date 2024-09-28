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
/// プレイヤー情報を描画
/// </summary>
/// <param name="pos">座標</param>
/// <param name="hp">現在のHP</param>
void DebugDraw::DrawPlayerInfo(const VECTOR pos, int hp, std::string state)
{
	DrawFormatString(0, 20, TextColor::kColorW, "座標(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 40, TextColor::kColorW, "HP:%2f", hp);
	DrawFormatString(0, 60, TextColor::kColorW, "状態:%s", state.c_str());
}