#pragma once
#include "ObjectBase.h"

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
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadLocationData();


protected:
	// 配置情報データ
	struct LocationData
	{
		std::string name;	// 名前
		std::string tag;	// タグ
		VECTOR pos;			// 座標
		VECTOR rot;			// 回転
		VECTOR scale;		// スケール
	};
	std::vector<LocationData> m_locationData;

protected:
	std::unordered_map<std::string, int> m_objHandle; // 読み込むオブジェクトのハンドル
	WeaponData m_weaponData;
	int m_locationDataHandle;	// 読み込むデータ
	int m_durability; // 武器の耐久力
};