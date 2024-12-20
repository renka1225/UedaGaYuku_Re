﻿#include "Item.h"
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

void EnemyStateDeath::Update(Stage& pStage, Player& pPlayer)
{
	EnemyStateBase::Update(pStage, pPlayer);
	m_pEnemy->Move(VGet(0.0f, 0.0f, 0.0f), pStage);   // 移動情報を反映する

	m_deathTime--;
	if (m_deathTime <= 0.0f)
	{
		DropItem(pStage); // アイテムをドロップする

		m_pEnemy->SetIsDead(true);
		MV1DetachAnim(m_pEnemy->GetHandle(),m_pEnemy->GetAnimIndex(AnimName::kDown));
	}
}

void EnemyStateDeath::DropItem(Stage& pStage)
{
	if (m_pEnemy == nullptr) return;

	// 確率でお金かアイテムをドロップする
	int randNum = GetRand(30);

	if (randNum <= 5)
	{
		// 敵がいた場所にお金をドロップする
		pStage.SetDropMoney(m_pEnemy->GetPos(), 500);
	}
	else if (randNum <= 10)
	{
		pStage.SetDropMoney(m_pEnemy->GetPos(), 1000);
	}
	else if(randNum <= 15)
	{
		pStage.SetDropMoney(m_pEnemy->GetPos(), 1500);
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
