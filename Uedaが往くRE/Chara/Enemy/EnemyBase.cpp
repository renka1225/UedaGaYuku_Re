#include "DebugDraw.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"

namespace
{
	constexpr float kScale = 0.15f;	 // モデルの拡大率
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase()
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
}

/// <summary>
/// 初期化
/// </summary>
void EnemyBase::Init()
{
	CharacterBase::Init();
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	//m_pState = std::make_shared<EnemyStateBase>(shared_from_this());
	//m_pState->m_nextState = m_pState;

	//auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pState);
	//state->Init();
}

/// <summary>
/// 更新
/// </summary>
void EnemyBase::Update(Stage& stage)
{
	CharacterBase::Update();

	//// 前のフレームと違うstateの場合
	//if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	//{
	//	// stateを変更する
	//	m_pState = m_pState->m_nextState;
	//	m_pState->m_nextState = m_pState;
	//}

	UpdateCol(); // 当たり判定の位置更新
}

/// <summary>
/// 描画
/// </summary>
void EnemyBase::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	//debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName()); // プレイヤーの情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_updateCol.bodyStartPos, m_updateCol.bodyEndPos, m_colData.bodyRadius); // 全身
	//debug.DrawAimCol(m_updateCol.armStartPos, m_updateCol.armEndPos, m_colData.aimRadius);   // 腕
	//debug.DrawLegCol(m_updateCol.legStartPos, m_updateCol.legEndPos, m_colData.legRadius);   // 脚
#endif
}
