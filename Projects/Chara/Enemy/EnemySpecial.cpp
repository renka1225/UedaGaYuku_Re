#include "EnemySpecial.h"

namespace
{
	const VECTOR kBobSpwnPos = VGet(9546.0f, 45.0f, 4732.0f);	// Bobのスポーン位置
	const VECTOR kSatoSpwnPos = VGet(10324.0f, 45.0f, 7995.0f);	// Satoのスポーン位置
	const VECTOR kAbeSpwnPos = VGet(5655.0f, 43.0f, 6947.0f);	// Abeのスポーン位置
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
	SetEnemySpawnInitPos();
}

void EnemySpecial::Update(Stage& pStage, Player& pPlayer)
{
	CharacterBase::Update();
	CommonUpdate(pStage, pPlayer);
	m_isPossibleMove = false;
}

void EnemySpecial::Draw(Player& player)
{
	EnemyBase::Draw(player);
	CharacterBase::Draw();
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
