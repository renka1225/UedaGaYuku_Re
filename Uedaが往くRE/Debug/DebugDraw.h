#pragma once
#include "DxLib.h"

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
	void DrawInfo(const VECTOR pos);
};

