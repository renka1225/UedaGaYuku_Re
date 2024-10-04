#pragma once
#include "DxLib.h"

class Input;
class Player;
class Stage;

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Update(Input& input, const Player& player, const Stage& stage);

	VECTOR GetAngle() const { return m_target; } // カメラの角度を取得
	VECTOR GetPos() const { return m_pos; }		 // カメラ位置を取得
	float GetAngleH() const{ return m_angleH; }	 // カメラの水平角度を取得

private:
	void FixCameraPos();					 // カメラ位置補正
	void CheckCameraCol(const Stage& stage); // カメラの当たり判定をチェックする

private:
	VECTOR m_pos;					// カメラ座標
	VECTOR m_target;				// 注視点座標
	float m_angleH;					// 水平角度
	float m_angleV;					// 垂直角度
	MATRIX m_rotY;					// Y軸回転行列
	MATRIX m_rotZ;					// Z軸回転行列
	DINPUT_JOYSTATE m_analogInput; 	// アナログパッド情報取得
	int m_lightHandle;				// ライトハンドル
};

