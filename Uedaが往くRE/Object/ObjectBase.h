#pragma once
#include "DxLib.h"
#include "Stage.h"
#include <memory>

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	void Move(const VECTOR& moveVec, Stage& stage);

	VECTOR GetPos() const { return m_pos; }	// 座標を取得


protected:
	std::shared_ptr<Stage> m_pStage;	// ステージのポインタ
	VECTOR m_pos;						// 現在位置
	VECTOR m_moveDir;					// 向くべき方向のベクトル
	float m_gravity;					// 重力
	int m_modelHandle;					// 3Dモデルのハンドル
};

