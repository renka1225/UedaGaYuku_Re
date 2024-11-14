#include "Game.h"
#include "Item.h"
#include "EnemyBase.h"
#include "EnemyStateDeath.h"
#include <random>

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

void EnemyStateDeath::Update(Stage& stage, Player& pPlayer)
{
	EnemyStateBase::Update(stage, pPlayer);
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	m_deathTime--;
	if (m_deathTime <= 0.0f)
	{
		m_pEnemy->SetIsDead(true);

		DropMoney(stage);
	}
}

void EnemyStateDeath::DropMoney(Stage& stage)
{
	if (m_pEnemy == nullptr) return;

	// 確率でお金かアイテムをドロップする
	int randNum = GetRand(20);

	if (randNum <= 5)
	{
		// 敵がいた場所にお金をドロップする
		stage.SetDropMoney(m_pEnemy->GetPos(), 500);
	}
	else if (randNum <= 10)
	{
		stage.SetDropMoney(m_pEnemy->GetPos(), 1000);
	}
	else if(randNum <= 13)
	{
		stage.SetDropMoney(m_pEnemy->GetPos(), 1500);

	}
	else
	{
		// ドロップするアイテムをランダムで決める
		int itemKind;
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution urdIndex(1, static_cast<int>(Item::ItemType::kItemKind));
		itemKind = urdIndex(mt);

		stage.SetDropItem(m_pEnemy->GetPos(), itemKind);
	}
}
