#include "DxLib.h"
#include "ObjectBase.h"

namespace
{
	constexpr float kGravity = -3.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
ObjectBase::ObjectBase():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_gravity(kGravity)
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
	//m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void ObjectBase::Draw()
{
}
