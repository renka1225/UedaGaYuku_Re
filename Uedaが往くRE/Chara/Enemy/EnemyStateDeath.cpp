#include "Game.h"
#include "EnemyBase.h"
#include "EnemyStateDeath.h"

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
	// TODO:確率でお金をドロップする
	int randNum = GetRand(10);

	if (randNum <= 7)
	{
		// 敵がいた場所にお金をドロップする
		stage.SetDropMoney(m_pEnemy->GetPos(), 100);
	}
	else if (randNum <= 9)
	{
		stage.SetDropMoney(m_pEnemy->GetPos(), 300);
	}
	else
	{
		stage.SetDropMoney(m_pEnemy->GetPos(), 500);

	}
}
