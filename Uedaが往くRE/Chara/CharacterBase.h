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

public:
	// ステータス構造体
	struct Status
	{
		float maxHp;		// 最大HP
		float walkSpeed;	// 歩き速度
		float runSpeed;		// 走り速度
	};

protected:
	Status m_status;	// ステータス
	float m_hp;			// HP
	VECTOR m_pos;		// 現在位置
	int m_modelHandle;	// キャラクターのモデル
};

