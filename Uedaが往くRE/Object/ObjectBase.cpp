#include "ObjectBase.h"

namespace
{
	constexpr float kGravity = -15.0f; // 重力
}

/// <summary>
/// コンストラクタ
/// </summary>
ObjectBase::ObjectBase():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_gravity(kGravity),
	m_modelHandle(-1)
{
}

/// <summary>
/// デストラクタ
/// </summary>
ObjectBase::~ObjectBase()
{
}

/// <summary>
/// 初期化
/// </summary>
void ObjectBase::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void ObjectBase::Update()
{
	// 重力を足す
	m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void ObjectBase::Draw()
{
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="moveVec">移動量</param>
void ObjectBase::Move(const VECTOR& moveVec, Stage& stage)
{
	// 移動中のみ向きを更新する
	if (VSize(moveVec) > 0.0f)
	{
		m_moveDir = VNorm(moveVec);
	}

	// ステージと当たり判定を行って座標を保存する
	m_pos = stage.CheckObjectCol(*this, moveVec);

	m_pos = VAdd(m_pos, moveVec);
	MV1SetPosition(m_modelHandle, m_pos);
}
