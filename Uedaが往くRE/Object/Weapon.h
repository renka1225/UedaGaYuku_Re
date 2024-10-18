#pragma once
#include "ObjectBase.h"

/// <summary>
/// 武器クラス
/// </summary>
class Weapon : public ObjectBase
{
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
		int durability;		// 耐久力
	};
	std::vector<LocationData> m_locationData;

protected:
	std::unordered_map<std::string, int> m_objHandle; // 読み込むオブジェクトのハンドル
	int m_locationDataHandle;	// 読み込むデータ
};