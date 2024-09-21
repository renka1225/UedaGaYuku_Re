#pragma once
#include "DxLib.h"

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class CharacterBase
{
public:
	CharacterBase();
	~CharacterBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	VECTOR GetPos() const { return m_pos; } // 現在位置取得

protected:
	VECTOR m_pos;		// 現在位置
	int m_modelHandle;	// キャラクターのモデル
};

