#pragma once

class ObjectBase;

/// <summary>
/// ステージの更新と描画を行うクラス
/// </summary>
class Stage
{
public:
	Stage();
	virtual ~Stage();
	void Draw();
	// オブジェクトと当たり判定をして、補正した移動先のポジションを返す
	VECTOR CheckObjectCol(ObjectBase& obj, const VECTOR& moveVec);

	// ステージモデルを取得
	int GetStageHandle() const { return m_stageHandle; }

private:
	// 検出されたポリゴンが壁ポリゴンか床ポリゴンかを判断する
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);
	// オブジェクトと壁ポリゴンとの当たりをチェックする
	VECTOR CheckHitPlayerWithWall(ObjectBase& player, const VECTOR& checkPosition);
	// オブジェクトと床ポリゴンとの当たりをチェックする
	VECTOR CheckHitPlayerWithFloor(ObjectBase& player, const VECTOR& checkPosition);

private:
	// ステージ情報
	int m_wallNum;			// 壁ポリゴンの数
	int	m_floorNum;			// 床ポリゴンの数
	int m_stageHandle;		// ステージの3Dモデル
	int m_skydoomHandle;	// スカイドームの3Dモデル

	static const int MaxHitColl = 2048;			// 処理するコリジョンポリゴンの最大数
	MV1_COLL_RESULT_POLY* m_wall[MaxHitColl];	// 壁ポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* m_floor[MaxHitColl];	// 床ポリゴンの構造体のアドレスを保存しておくためのポインタ配列
};

