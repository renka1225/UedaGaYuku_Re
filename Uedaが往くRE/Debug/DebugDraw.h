#pragma once
#include "DxLib.h"
#include <string>

/// <summary>
/// デバッグ表示
/// </summary>
class DebugDraw
{
public:
	DebugDraw() {}

	~DebugDraw() {}

	/// <summary>
	/// プレイヤー情報を描画
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="hp">現在のHP</param>
	/// <param name="state">現在の状態/param>
	void DrawPlayerInfo(const VECTOR pos, float hp, std::string state, bool isGrabWeapon);

	/// <summary>
	/// 敵情報を描画
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="hp">現在のHP</param>
	/// <param name="state">現在の状態</param>
	void DrawEnemyInfo(const VECTOR pos, float hp, std::string state);

	/// <summary>
	/// 武器情報を描画
	/// </summary>
	/// <param name="name">武器名</param>
	/// <param name="tag">武器の種類</param>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転率</param>
	/// <param name="scale">サイズ</param>
	/// <param name="durability">耐久力</param>
	void DrawWeaponInfo(std::string name, std::string tag, VECTOR pos, VECTOR rot, VECTOR scale, int durability);

	/// <summary>
	/// 全身の当たり判定を描画
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="r">半径</param>
	void DrawBodyCol(VECTOR startPos, VECTOR endPos, float r);

	/// <summary>
	/// 腕の当たり判定を描画
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="r">半径</param>
	void DrawAimCol(VECTOR startPos, VECTOR endPos, float r);

	/// <summary>
	/// 脚の当たり判定を描画
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="r">半径</param>
	void DrawLegCol(VECTOR startPos, VECTOR endPos, float r);

	/// <summary>
	/// 武器の当たり判定を描画
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="r">半径</param>
	void DrawWeaponCol(VECTOR startPos, VECTOR endPos, float r);
};

