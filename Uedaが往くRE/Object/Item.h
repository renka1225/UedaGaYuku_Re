#pragma once
#include <string>
#include <map>

class Player;

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class Item
{
public:
	/// <summary>
	/// アイテムの種類
	/// </summary>
	enum ItemType
	{
		kHpSmall,
		kHpMiddle,
		kHpLarge,
		kGaugeSmall,
		kGaugeLarge,
		kHpGaugeSmall,
		kHpGaugeLarge,
		kAtkSmall,
		kAtkLarge,
		kDefSmall,
		kDefLarge,
		kItemKind
	};

	/// <summary>
	/// アイテムのデータ
	/// </summary>
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
	Item();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Item();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アイテムの効果を適用する
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="m_select">選択したアイテム番号</param>
	void ApplyEffect(Player& player, int m_select);

	/// <summary>
	/// アイテムの情報を取得する
	/// </summary>
	/// <param name="itemType">取得するアイテムの種類</param>
	/// <returns>アイテムの情報</returns>
	// MEMO:vector.at()で指定した要素を返す
	ItemData GetItemData(int itemType) const { return m_itemData.at(static_cast<ItemType>(itemType)); }

private:
	ItemType m_itemType; // アイテムの種類
	std::map<ItemType, ItemData> m_itemData; // アイテムの情報
};

