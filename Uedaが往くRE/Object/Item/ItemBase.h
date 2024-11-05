#pragma once
#include <string>
#include <map>

/// <summary>
/// アイテムの基底クラス
/// </summary>
class ItemBase
{
public:
	struct ItemData
	{
		std::string itemName;	 // アイテム名
		std::string itemExplain; // アイテムの説明
		float recoveryHP;		 // HP回復量
		float recoveryGauge;	 // ゲージ回復量
		float atkUp;			 // 攻撃力アップ量
		float difUp;			 // 防御力アップ量
		int time;				 // 効果時間
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ItemBase();

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

private:
	std::map<std::string, ItemData> m_itemData; // アイテムの情報
};

