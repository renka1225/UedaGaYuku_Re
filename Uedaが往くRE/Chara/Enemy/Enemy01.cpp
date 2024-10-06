#include "DxLib.h"
#include "LoadCsv.h"
#include "Enemy01.h"

namespace
{
	const std::string kCharaId = "enemy_01";						// キャラクターのID名
	const char* kModelFileName = ("data/model/chara/enemy_01.mv1");	// モデルのファイル名
	const VECTOR kInitPos = VGet(7600.0, 40.0f, 5300.0f);	// 初期位置
}

/// <summary>
/// コンストラクタ
/// </summary>
Enemy01::Enemy01()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData, kCharaId);
}

/// <summary>
/// デストラクタ
/// </summary>
Enemy01::~Enemy01()
{
	CharacterBase::~CharacterBase();
}

/// <summary>
/// 初期化
/// </summary>
void Enemy01::Init()
{
	EnemyBase::Init();
}

/// <summary>
/// 更新
/// </summary>
void Enemy01::Update(Stage& stage)
{
	EnemyBase::Update(stage);
}

/// <summary>
/// 描画
/// </summary>
void Enemy01::Draw()
{
	EnemyBase::Draw();
}
