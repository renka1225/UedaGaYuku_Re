﻿#pragma once
#include "DxLib.h"
#include "Stage.h"
#include <memory>
#include <unordered_map>
#include <string>

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectBase() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="moveVec">移動ベクトル</param>
	/// <param name="stage">ステージ参照</param>
	/// <param name="isChangeDir">モデルの角度を更新するか</param>
	void Move(const VECTOR& moveVec, Stage& stage, bool isChangeDir = true);

	/// <summary>
	/// 指定したフレームのフレーム番号を取得する
	/// </summary>
	/// <param name="frameName">フレーム名</param>
	/// <returns>フレーム番号</returns>
	int GetModelFrameIndex(const char* frameName);

	/// <summary>
	/// 指定したフレーム番号の座標を取得する
	/// </summary>
	/// <param name="frameName">フレーム名</param>
	/// <returns>座標</returns>
	VECTOR GetModelFramePos(const char* frameName);

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>オブジェクトの座標</returns>
	VECTOR GetPos() const { return m_pos; }

	/// <summary>
	/// 向くべき方向のベクトルを取得
	/// </summary>
	/// <returns>方向ベクトル</returns>
	VECTOR GetDir() const { return m_moveDir; }

protected:
	std::shared_ptr<Stage> m_pStage; // ステージのポインタ
	VECTOR m_pos;		// 現在位置
	VECTOR m_moveDir;	// 向くべき方向のベクトル
	float m_gravity;	// 重力
	int m_modelHandle;	// 3Dモデルのハンドル
};

