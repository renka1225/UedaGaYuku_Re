#pragma once
#include "DxLib.h"
#include <string>

/// <summary>
/// デバッグ表示
/// </summary>
class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();
	void Init();
	void Update();
	void DrawPlayerInfo(const VECTOR pos, float hp, std::string state); // プレイヤーの情報を描画
	void DrawEnemyInfo(const VECTOR pos, float hp, std::string state);  // 敵の情報を描画
	void DrawBodyCol(VECTOR startPos, VECTOR endPos, float r);			// 全身の当たり判定を描画
	void DrawAimCol(VECTOR startPos, VECTOR endPos, float r);			// 腕の当たり判定を描画
	void DrawLegCol(VECTOR startPos, VECTOR endPos, float r);			// 脚の当たり判定を描画
};

