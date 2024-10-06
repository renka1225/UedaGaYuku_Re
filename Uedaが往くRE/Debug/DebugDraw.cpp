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
void DebugDraw::DrawPlayerInfo(const VECTOR pos, float hp, std::string state)
{
	DrawFormatString(0, 20, Color::kColorW, "プレイヤー座標(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 40, Color::kColorW, "プレイヤーHP:%2f", hp);
	DrawFormatString(0, 60, Color::kColorW, "プレイヤー状態:%s", state.c_str());
}

/// <summary>
/// 敵情報を描画
/// </summary>
/// <param name="pos"></param>
/// <param name="hp"></param>
/// <param name="state"></param>
void DebugDraw::DrawEnemyInfo(const VECTOR pos, float hp, std::string state)
{
	DrawFormatString(0, 80, Color::kColorW, "敵座標(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 100, Color::kColorW, "敵HP:%2f", hp);
	DrawFormatString(0, 120, Color::kColorW, "敵状態:%s", state.c_str());
}

/// <summary>
/// 全身の当たり判定を描画
/// </summary>
/// <param name="startPos">開始位置</param>
/// <param name="endPos">終了位置</param>
/// <param name="r">半径</param>
void DebugDraw::DrawBodyCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorP, Color::kColorBK, false);
}

/// <summary>
/// 腕の当たり判定を描画
/// </summary>
/// <param name="startPos">開始位置</param>
/// <param name="endPos">終了位置</param>
/// <param name="r">半径</param>
void DebugDraw::DrawAimCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorLB, Color::kColorBK, false);
}

/// <summary>
/// 脚の当たり判定を描画
/// </summary>
/// <param name="startPos">開始位置</param>
/// <param name="endPos">終了位置</param>
/// <param name="r">半径</param>
void DebugDraw::DrawLegCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorY, Color::kColorBK, false);
}