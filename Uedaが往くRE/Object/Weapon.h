#pragma once
#include "ObjectBase.h"

class Player;
class Stage;

/// <summary>
/// 武器クラス
/// </summary>
class Weapon : public ObjectBase
{
public:
	// 武器の情報
	struct WeaponData
	{
		int durability;		// 耐久力
		VECTOR colStartPos; // 武器の当たり判定始点
		VECTOR colEndPos;	// 武器の当たり判定終点
		float colRadius;	// 武器の当たり判定半径
	};

	// 当たり判定更新データ
	struct UpdateColData
	{
		VECTOR colStartPos; // 武器の当たり判定始点
		VECTOR colEndPos;	// 武器の当たり判定終点
		float colRadius;	// 武器の当たり判定半径
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Weapon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Weapon();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(Player& player, Stage& stage);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 武器の当たり判定をチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="eCapPosTop">当たり判定カプセルの始点</param>
	/// <param name="eCapPosBottom">当たり判定カプセルの終点</param>
	/// <param name="eCapRadius">当たり判定カプセルの半径</param>
	void CheckWeaopnCol(ObjectBase& obj, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius);

private:
	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadLocationData();

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void UpdateCol(auto& loc);

	/// <summary>
	/// 武器を持った際に位置を更新する
	/// </summary>
	/// <param name="modelHandle">キャラクターの3Dモデル</param>
	/// <param name="frameName">セットするフレーム名</param>
	/// <param name="setModelHandle">セットする武器のモデル</param>
	/// <param name="setModelHandle">配置データ</param>
	void SetModelFramePos(int modelHandle, const char* frameName, int setModelHandle, auto& loc);

protected:
	// 配置情報データ
	struct LocationData
	{
		std::string name;	// 名前
		std::string tag;	// タグ
		VECTOR pos;			// 座標
		VECTOR rot;			// 回転
		VECTOR scale;		// スケール
		VECTOR initPos;		// 初期位置
		VECTOR initRot;		// 初期回転量
	};
	std::vector<LocationData> m_locationData;

protected:
	std::unordered_map<std::string, int> m_objHandle; // 読み込むオブジェクトのハンドル
	WeaponData m_weaponData;	// 武器のデータ
	UpdateColData m_updateCol;	// 更新後の当たり判定データ
	int m_locationDataHandle;	// 読み込む配置データ
	int m_durability; // 武器の耐久力
};