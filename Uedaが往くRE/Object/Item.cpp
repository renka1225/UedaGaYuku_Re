#include "DebugDraw.h"
#include "DxLib.h"
#include "LoadCsv.h"
#include "CharacterBase.h"
#include "Player.h"
#include "Item.h"

namespace
{
	// アイテムモデルのパス
	const std::string kItemHandleFile = "data/model/item/";
	const std::map<int, std::string> kItemHandlePath
	{
		//{ Item::ItemType::kHpSmall,  kItemHandleFile + "hpSmall.mv1"},
		//{ Item::ItemType::kHpMiddle,  kItemHandleFile + "hpMiddle.mv1"},
		//{ Item::ItemType::kHpLarge,  kItemHandleFile + "hpLarge.mv1"},
		//{ Item::ItemType::kGaugeSmall,  kItemHandleFile + "gaugeSmall.mv1"},
		//{ Item::ItemType::kGaugeLarge,  kItemHandleFile + "gaugeLarge.mv1"},
		//{ Item::ItemType::kHpGaugeSmall,  kItemHandleFile + "hpGaugeSmall.mv1"},
		//{ Item::ItemType::kHpGaugeLarge,  kItemHandleFile + "hpGaugeLarge.mv1"},
		//{ Item::ItemType::kAtkSmall,  kItemHandleFile + "atkSmall.mv1"},
		//{ Item::ItemType::kAtkLarge,  kItemHandleFile + "atkLarge.mv1"},

		// 仮モデル
		{ Item::ItemType::kHpSmall,  kItemHandleFile + "item1.mv1"},
		{ Item::ItemType::kHpMiddle,  kItemHandleFile + "item1.mv1"},
		{ Item::ItemType::kHpLarge,  kItemHandleFile + "item1.mv1"},
		{ Item::ItemType::kGaugeSmall,  kItemHandleFile + "item2.mv1"},
		{ Item::ItemType::kGaugeLarge,  kItemHandleFile + "item2.mv1"},
		{ Item::ItemType::kHpGaugeSmall,  kItemHandleFile + "item3.mv1"},
		{ Item::ItemType::kHpGaugeLarge,  kItemHandleFile + "item3.mv1"},
		{ Item::ItemType::kAtkSmall,  kItemHandleFile + "item4.mv1"},
		{ Item::ItemType::kAtkLarge,  kItemHandleFile + "item4.mv1"},
	};

	const float kItemColRadius = 10.0f;	// アイテムの当たり判定の半径
}

Item::Item():
	m_itemType()
{
	LoadCsv::GetInstance().LoadItemData(m_itemData);

	m_modelHandle.resize(Item::ItemType::kItemKind);
	for (int i = 0; i < Item::ItemType::kItemKind; i++)
	{
		m_modelHandle[i] = MV1LoadModel(kItemHandlePath.at(i).c_str());
	}
}

Item::~Item()
{
	for (auto& handle : m_modelHandle)
	{
		MV1DeleteModel(handle);
	}
}

void Item::Init()
{
}

void Item::Update(Player& pPlayer)
{
	ObjectBase::Update();
	//Move(VGet(0.0f, 0.0f, 0.0f), pStage);   // 移動情報を反映する

	for (auto& item : m_dropItem)
	{
		MV1SetPosition(m_modelHandle[item.itemType], item.pos);
	}

	CheckPlayerCol(pPlayer);
}

void Item::Draw()
{
	// ドロップ中のアイテムを表示する
	for (auto& item : m_dropItem)
	{
		MV1DrawModel(m_modelHandle[item.itemType]);

#ifdef _DEBUG
		DebugDraw debug;
		for (auto& item : m_dropItem)
		{
			debug.DrawItemCol(item.pos, kItemColRadius);
		}
#endif
	}
}

void Item::SetDropItem(VECTOR enemyPos, int itemType)
{
	printfDx("%dのアイテムをセット\n", itemType);
	m_dropItem.push_back({ itemType, enemyPos});
}

void Item::CheckPlayerCol(Player& pPlayer)
{
	// プレイヤーの当たり判定情報を取得
	auto playerCol = pPlayer.GetCol(CharacterBase::CharaType::kPlayer);

	for (auto it = m_dropItem.begin(); it != m_dropItem.end();)
	{
		// アイテムとプレイヤーの当たり判定を行う
		bool isHit = HitCheck_Sphere_Capsule(it->pos, kItemColRadius, playerCol.bodyUpdateStartPos, playerCol.bodyUpdateEndPos, playerCol.bodyRadius);

		// 当たった場合
		if (isHit)
		{
			// プレイヤーのアイテムを追加する
			pPlayer.AddItem(it->itemType);
			// 取得したアイテムを削除する
			it = m_dropItem.erase(it);
		}
		else
		{
			// 次のアイテムをチェックする
			it++;
		}
	}
}

void Item::ApplyEffect(Player& pPlayer, int select)
{
	if (select == -1) return; // 選択したアイテムを持っていない場合は飛ばす

	ItemType itemKind = static_cast<ItemType>(select);

	switch (itemKind)
	{
	case ItemType::kHpSmall:
		pPlayer.RecoveryHp(m_itemData[ItemType::kHpSmall].recoveryHP);
		break;
	case ItemType::kHpMiddle:
		pPlayer.RecoveryHp(m_itemData[ItemType::kHpMiddle].recoveryHP);
		break;
	case ItemType::kHpLarge:
		pPlayer.RecoveryHp(m_itemData[ItemType::kHpLarge].recoveryHP);
		break;
	case ItemType::kGaugeSmall:
		pPlayer.RecoveryGauge(m_itemData[ItemType::kGaugeSmall].recoveryGauge);
		break;
	case ItemType::kGaugeLarge:
		pPlayer.RecoveryGauge(m_itemData[ItemType::kGaugeLarge].recoveryGauge);
		break;
	case ItemType::kHpGaugeSmall:
		pPlayer.RecoveryHpGauge(m_itemData[ItemType::kHpGaugeSmall].recoveryHP, m_itemData[ItemType::kHpGaugeSmall].recoveryGauge);
		break;
	case ItemType::kHpGaugeLarge:
		pPlayer.RecoveryHpGauge(m_itemData[ItemType::kHpGaugeLarge].recoveryHP, m_itemData[ItemType::kHpGaugeLarge].recoveryGauge);
		break;
	case ItemType::kAtkSmall:
		pPlayer.AtkUp(m_itemData[ItemType::kAtkSmall].atkUp, m_itemData[ItemType::kAtkSmall].time);
		break;
	case ItemType::kAtkLarge:
		pPlayer.AtkUp(m_itemData[ItemType::kAtkLarge].atkUp, m_itemData[ItemType::kAtkLarge].time);
		break;
	}
}
