#include "DxLib.h"
#include "LoadCsv.h"
#include "Enemy01.h"

namespace
{
	const std::string kCharaId = "enmey_01";				// キャラクターのID名
	const VECTOR kInitPos = VGet(7600.0, 40.0f, 5300.0f);	// 初期位置
}

/// <summary>
/// コンストラクタ
/// </summary>
Enemy01::Enemy01()
{
	m_modelHandle = MV1LoadModel("data/model/chara/enemy_01.mv1");

	MV1SetPosition(m_modelHandle, kInitPos);

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData, kCharaId);
}

/// <summary>
/// デストラクタ
/// </summary>
Enemy01::~Enemy01()
{
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
	DrawFormatString(0, 100, 0xffffff, "敵座標(X:%.2f, Y:%.2f, Z:%.2f", m_pos.x, m_pos.y, m_pos.z);
}
