#include "Item.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyStateDeath.h"
#include <random>

namespace
{
	constexpr int kMaxItemDrop = 30;	// アイテムがドロップする最大確率
	constexpr int kMoneyItemDrop = 10;	// お金がドロップする最大確率
	constexpr int kMinDropMoney = 500;	// ドロップする最小金額
	constexpr int kMaxDropMoney = 2000;	// ドロップする最大金額
}

EnemyStateDeath::EnemyStateDeath(std::shared_ptr<EnemyBase> pEnemy) :
	EnemyStateBase(pEnemy),
	m_deathTime(0.0f)
{
}

void EnemyStateDeath::Init()
{
	m_pEnemy->ChangeAnim(AnimName::kDown);
	m_deathTime = m_pEnemy->GetAnimTotalTime(AnimName::kDown);
}

void EnemyStateDeath::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), pStage);   // 移動情報を反映する

	m_deathTime--;
	if (m_deathTime <= 0.0f)
	{
		DropItem(pPlayer); // アイテムをドロップする

		m_pEnemy->SetIsDead(true);
		MV1DetachAnim(m_pEnemy->GetHandle(),m_pEnemy->GetAnimIndex(AnimName::kDown));
	}
}

void EnemyStateDeath::DropItem(Player& pPlayer)
{
	if (m_pEnemy == nullptr) return;

	// 確率でお金かアイテムをドロップする
	int randNum = GetRand(kMaxItemDrop);

	if (randNum <= kMoneyItemDrop)
	{
		// ドロップするする金額を決める
		int dropMoney = GetRand((kMaxDropMoney - kMinDropMoney)) + kMinDropMoney;

		pPlayer.AddMoney(dropMoney);
	}
	else
	{
		// ドロップするアイテムをランダムで決める
		int itemKind;
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution urdIndex(0, static_cast<int>(Item::ItemType::kItemKind - 1));
		itemKind = urdIndex(mt);

		// ドロップしたアイテムの情報を渡す
		m_pEnemy->GetItemPointer()->SetDropItem(itemKind, m_pEnemy->GetPos());
	}
}
