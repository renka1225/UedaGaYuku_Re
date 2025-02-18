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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="pStage">ステージ参照</param>
	void Update(Input& input, const Player& pPlayer, const Stage& pStage);

	/// <summary>
	/// バトル開始時カメラ位置を更新する
	/// </summary>
	void BattleStart();

	/// <summary>
	/// 特殊バトルのカメラをセットする
	/// </summary>
	void SetSpecialBattleInit();

	/// <summary>
	/// バトル開始時のカメラ演出
	/// </summary>
	void BattleStartProduction();

	/// <summary>
	/// カメラを動かせるかどうかセットする
	/// </summary>
	/// <param name="isMove">カメラを移動できるかどうか</param>
	void SetIsCameraMove(bool isMove) { m_isMove = isMove; }

	/// <summary>
	/// カメラの角度を取得
	/// </summary>
	/// <returns>カメラ角度</returns>
	VECTOR GetAngle() const { return m_target; }

	/// <summary>
	/// カメラ位置を取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	VECTOR GetPos() const { return m_pos; }

	/// <summary>
	/// カメラの水平角度を取得
	/// </summary>
	/// <returns>カメラの水平角度</returns>
	float GetAngleH() const{ return m_angleH; }

	/// <summary>
	/// カメラの垂直角度を取得
	/// </summary>
	/// <returns>カメラの垂直角度</returns>
	float GetAngleV() const { return m_angleV; }

private:
	/// <summary>
	/// カメラの角度を更新
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// カメラ位置補正
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void FixCameraPos(const Player& pPlayer);

	/// <summary>
	/// カメラの線形補間を行う
	/// </summary>
	/// <returns>補間後のカメラ座標</returns>
	VECTOR LerpCamera();

	/// <summary>
	/// カメラの当たり判定をチェックする
	/// </summary>
	/// <param name="pStage">ステージ参照</param>
	void CheckCameraCol(const Stage& pStage);

private:
	VECTOR m_prevPlayerPos;			// 移動前のプレイヤー座標
	VECTOR m_pos;					// カメラ座標
	VECTOR m_target;				// 注視点座標
	float m_dist;					// カメラ距離
	float m_angleH;					// 水平角度
	float m_angleV;					// 垂直角度
	MATRIX m_rotY;					// Y軸回転行列
	MATRIX m_rotZ;					// Z軸回転行列
	DINPUT_JOYSTATE m_analogInput; 	// アナログパッド情報取得
	int m_lightHandle;				// ライトハンドル
	bool m_isMove;					// カメラを動かせるかどうか
};