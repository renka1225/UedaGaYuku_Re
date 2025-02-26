#include "EnemySpecial.h"

namespace
{
	const VECTOR kBobSpwnPos = VGet(9500.0f, 45.0f, 2854.0f);  // Bobのスポーン位置
	const VECTOR kSatoSpwnPos = VGet(8994.0f, 45.0f, 2854.0f); // Satoのスポーン位置
	const VECTOR kAbeSpwnPos = VGet(8994.0f, 45.0f, 2854.0f);  // Abeのスポーン位置
}

EnemySpecial::EnemySpecial(std::shared_ptr<UiBar> pUi, std::shared_ptr<Item> pItem, Player& pPlayer):
	EnemyBase(pUi, pItem, pPlayer)
{
}

EnemySpecial::~EnemySpecial()
{
	EnemyBase::~EnemyBase();
}

void EnemySpecial::Init()
{
	EnemyBase::Init();
}

void EnemySpecial::Update(Stage& pStage, Player& pPlayer)
{
	EnemyBase::Update(pStage, pPlayer);
}

void EnemySpecial::Draw(Player& player)
{
	EnemyBase::Draw(player);
}

void EnemySpecial::SetEnemySpawnInitPos()
{
	switch (m_enemyIndex)
	{
	case CharaType::kEnemy_bob:
		m_pos = kBobSpwnPos;
		break;
	case CharaType::kEnemy_sato:
		m_pos = kSatoSpwnPos;
		break;
	case CharaType::kEnemy_abe:
		m_pos = kAbeSpwnPos;
		break;
	default:
		break;
	}

	MV1SetPosition(m_modelHandle, m_pos);
}
