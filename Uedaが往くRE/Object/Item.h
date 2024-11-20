#pragma once
#include "ObjectBase.h"
#include <string>
#include <vector>
#include <map>

class Player;

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class Item : public ObjectBase
{
public:
	/// <summary>
	/// アイテムの種類
	/// </summary>
	enum ItemType
	{
		kHpSmall,		// HP回復(小)
		kHpMiddle,		// HP回復(中)
		kHpLarge,		// HP回復(大)
		kGaugeSmall,	// ゲージ回復(小)
		kGaugeLarge,	// ゲージ回復(大)
		kHpGaugeSmall,	// HP+ゲージ回復(小)
		kHpGaugeLarge,	// HP+ゲージ回復(大)
		kAtkSmall,		// 攻撃力アップ(小)
		kAtkLarge,		// 攻撃力アップ(大)
		kItemKind		// HP回復(小)
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
		int time;				 // 効果時間
		int itemHandle;			 // アイテムの画像
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
	virtual void Update(Player& pPlayer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵がドロップしたアイテムをセットする
	/// </summary>
	/// <param name="itemType">ドロップしたアイテムの種類</param>
	/// <param name="enemyPos">アイテムをドロップした敵の位置</param>
	void SetDropItem(int itemType, VECTOR enemyPos);

	/// <summary>
	/// プレイヤーとの当たり判定をチェックする
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void CheckPlayerCol(Player& pPlayer);

	/// <summary>
	/// アイテムの効果を適用する
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="select">選択したアイテム番号</param>
	void ApplyEffect(Player& pPlayer, int select);

	/// <summary>
	/// アイテムの情報を取得する
	/// </summary>
	/// <param name="itemType">取得するアイテムの種類</param>
	/// <returns>アイテムの情報</returns>
	// MEMO:vector.at()で指定した要素を返す
	ItemData GetItemData(int itemType) const { return m_itemData.at(static_cast<ItemType>(itemType)); }

private:
	std::vector<int> m_modelHandle;			 // モデルのハンドル
	std::map<ItemType, ItemData> m_itemData; // アイテムの情報
	ItemType m_itemType; // アイテムの種類
	
	// ドロップしたアイテムの情報
	struct DropItem
	{
		int itemType;	// アイテムの種類
		VECTOR pos;		// アイテム位置
	};
	std::vector<DropItem> m_dropItem;	// ドロップしたアイテムの情報を保存する
};

