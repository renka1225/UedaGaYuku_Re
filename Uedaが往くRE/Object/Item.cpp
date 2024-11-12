#include "DxLib.h"
#include "LoadCsv.h"
#include "Player.h"
#include "Item.h"

Item::Item():
	m_itemType()
{
	LoadCsv::GetInstance().LoadItemData(m_itemData);
}

Item::~Item()
{
}

void Item::Init()
{
}

void Item::Update()
{
}

void Item::Draw()
{
}

void Item::ApplyEffect(Player& player, int m_select)
{
	ItemType itemKind = static_cast<ItemType>(m_select);

	switch (itemKind)
	{
	case ItemType::kHpSmall:
		player.RecoveryHp(m_itemData[ItemType::kHpSmall].recoveryHP);
		break;
	case ItemType::kHpMiddle:
		player.RecoveryHp(m_itemData[ItemType::kHpMiddle].recoveryHP);
		break;
	case ItemType::kHpLarge:
		player.RecoveryHp(m_itemData[ItemType::kHpLarge].recoveryHP);
		break;
	case ItemType::kGaugeSmall:
		player.RecoveryGauge(m_itemData[ItemType::kGaugeSmall].recoveryGauge);
		break;
	case ItemType::kGaugeLarge:
		player.RecoveryGauge(m_itemData[ItemType::kGaugeLarge].recoveryGauge);
		break;
	case ItemType::kHpGaugeSmall:
		player.RecoveryHpGauge(m_itemData[ItemType::kHpGaugeSmall].recoveryHP, m_itemData[ItemType::kHpGaugeSmall].recoveryGauge);
		break;
	case ItemType::kHpGaugeLarge:
		player.RecoveryHpGauge(m_itemData[ItemType::kHpGaugeLarge].recoveryHP, m_itemData[ItemType::kHpGaugeLarge].recoveryGauge);
		break;
	case ItemType::kAtkSmall:
		player.AtkUp(m_itemData[ItemType::kAtkSmall].atkUp, m_itemData[ItemType::kAtkSmall].time);
		break;
	case ItemType::kAtkLarge:
		player.AtkUp(m_itemData[ItemType::kAtkLarge].atkUp, m_itemData[ItemType::kAtkLarge].time);
		break;
	}
}
