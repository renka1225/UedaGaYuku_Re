#include "DebugDraw.h"
#include "DxLib.h"
#include "LoadCsv.h"
#include "CharacterBase.h"
#include "Player.h"
#include "Item.h"

namespace
{
	// アイテムモデルのパス
	const std::string kItemModelFile = "data/model/item/";
	const std::map<int, std::string> kItemModelPath
	{
		//{ Item::ItemType::kHpSmall,  kItemModelFile + "hpSmall.mv1"},
		//{ Item::ItemType::kHpMiddle,  kItemModelFile + "hpMiddle.mv1"},
		//{ Item::ItemType::kHpLarge,  kItemModelFile + "hpLarge.mv1"},
		//{ Item::ItemType::kGaugeSmall,  kItemModelFile + "gaugeSmall.mv1"},
		//{ Item::ItemType::kGaugeLarge,  kItemModelFile + "gaugeLarge.mv1"},
		//{ Item::ItemType::kHpGaugeSmall,  kItemModelFile + "hpGaugeSmall.mv1"},
		//{ Item::ItemType::kHpGaugeLarge,  kItemModelFile + "hpGaugeLarge.mv1"},
		//{ Item::ItemType::kAtkSmall,  kItemModelFile + "atkSmall.mv1"},
		//{ Item::ItemType::kAtkLarge,  kItemModelFile + "atkLarge.mv1"},

		// 仮モデル
		{ Item::ItemType::kHpSmall,  kItemModelFile + "item1.mv1"},
		{ Item::ItemType::kHpMiddle,  kItemModelFile + "item2.mv1"},
		{ Item::ItemType::kHpLarge,  kItemModelFile + "item3.mv1"},
		{ Item::ItemType::kGaugeSmall,  kItemModelFile + "item4.mv1"},
		{ Item::ItemType::kGaugeLarge,  kItemModelFile + "item5.mv1"},
		{ Item::ItemType::kHpGaugeSmall,  kItemModelFile + "item6.mv1"},
		{ Item::ItemType::kHpGaugeLarge,  kItemModelFile + "item7.mv1"},
		{ Item::ItemType::kAtkSmall,  kItemModelFile + "item8.mv1"},
		{ Item::ItemType::kAtkLarge,  kItemModelFile + "item9.mv1"},
	};

	const float kItemColRadius = 5.0f;	// アイテムの当たり判定の半径
	const float kItemScale = 0.5;		// アイテムの拡大率
}

Item::Item():
	m_itemType()
{
	LoadCsv::GetInstance().LoadItemData(m_itemData);
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
	m_modelHandle.resize(Item::ItemType::kItemKind);
	for (int i = 0; i < m_modelHandle.size(); i++)
	{
		m_modelHandle[i] = MV1LoadModel(kItemModelPath.at(i).c_str());
		MV1SetScale(m_modelHandle[i], VGet(kItemScale, kItemScale, kItemScale));
	}
}

void Item::Update(Player& pPlayer)
{
	ObjectBase::Update();

	for (auto& item : m_dropItem)
	{
		MV1SetPosition(m_modelHandle[item.itemType], item.pos);
	}

	// プレイヤーとの当たり判定をチェックする
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

void Item::SetDropItem(int itemType, VECTOR enemyPos)
{
	m_dropItem.push_back({itemType, enemyPos});
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
			// すでに所持アイテム数が上限に達している場合は飛ばす
			if (!pPlayer.GetIsAddItem()) return;

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
