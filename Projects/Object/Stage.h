﻿#pragma once
#include "DxLib.h"
#include <memory>

class ObjectBase;
class Player;

/// <summary>
/// ステージの更新と描画を行うクラス
/// </summary>
class Stage
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage(std::shared_ptr<Player> player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Stage();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ステージ描画
	/// </summary>
	void DrawStage();

	/// <summary>
	/// スカイドーム描画
	/// </summary>
	void DrawSkyDoom();

	/// <summary>
	/// オブジェクトと当たり判定を行う
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="moveVec">移動ベクトル</param>
	/// <returns>補正した移動先のポジション</returns>
	VECTOR CheckObjectCol(ObjectBase& obj, const VECTOR& moveVec);

	/// <summary>
	/// ステージモデルを取得
	/// </summary>
	/// <returns>3Dモデルのハンドル</returns>
	int GetStageHandle() const { return m_stageHandle; }

private:
	/// <summary>
	/// 検出されたポリゴンが壁ポリゴンか床ポリゴンかを判断する
	/// </summary>
	/// <param name="hitDim">ヒットしたコリジョン情報</param>
	/// <param name="checkPosition">移動後の座標</param>
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);
	
	/// <summary>
	/// オブジェクトと壁ポリゴンとの当たりをチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="checkPosition">移動後の座標</param>
	/// <returns>補正すべきベクトル</returns>
	VECTOR CheckHitObjWithWall(ObjectBase& obj, const VECTOR& checkPosition);
	
	/// <summary>
	/// オブジェクトと床ポリゴンとの当たりをチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="checkPosition">移動後の座標</param>
	/// <returns>補正すべきベクトル</returns>
	VECTOR CheckHitObjWithFloor(ObjectBase& obj, const VECTOR& checkPosition);

private:

	std::shared_ptr<Player> m_pPlayer; // プレイヤーのポインタ

	float m_skydoomRotate;	// スカイドームの回転角度
	int m_wallNum;			// 壁ポリゴンの数
	int	m_floorNum;			// 床ポリゴンの数
	int m_stageHandle;		// ステージの3Dモデル
	int m_skydoomHandle;	// スカイドームの3Dモデル

	static const int MaxHitColl = 2048;			// 処理するコリジョンポリゴンの最大数
	MV1_COLL_RESULT_POLY* m_wall[MaxHitColl];	// 壁ポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* m_floor[MaxHitColl];	// 床ポリゴンの構造体のアドレスを保存しておくためのポインタ配列

};

