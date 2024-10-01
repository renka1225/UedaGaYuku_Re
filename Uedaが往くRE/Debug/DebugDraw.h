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
	void DrawPlayerInfo(const VECTOR pos, float hp, std::string state);
};

