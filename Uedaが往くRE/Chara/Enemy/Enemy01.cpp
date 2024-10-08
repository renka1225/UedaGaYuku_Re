#include "DxLib.h"
#include "LoadCsv.h"
#include "Enemy01.h"

// 定数
namespace
{
	const std::string kCharaId = "enemy_01";						// キャラクターのID名
	const char* kModelFileName = ("data/model/chara/enemy_01.mv1");	// モデルのファイル名
	const VECTOR kInitPos = VGet(7600.0, 40.0f, 5300.0f);			// 初期位置
}

Enemy01::Enemy01()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData, kCharaId);
}

Enemy01::~Enemy01()
{
	CharacterBase::~CharacterBase();
}

void Enemy01::Init()
{
	EnemyBase::Init();
}

void Enemy01::Update(Stage& stage)
{
	EnemyBase::Update(stage);
}

void Enemy01::Draw()
{
	EnemyBase::Draw();
}
