#include "ObjectBase.h"

// 定数
namespace
{
	constexpr float kGravity = -15.0f; // 重力
}

ObjectBase::ObjectBase():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_gravity(kGravity),
	m_modelHandle(-1)
{
}

void ObjectBase::Update()
{
	// 重力を足す
	m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));

	MV1SetPosition(m_modelHandle, m_pos);	// 位置を更新
}

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

int ObjectBase::GetModelFrameIndex(const char* frameName)
{
	return MV1SearchFrame(m_modelHandle, frameName);
}

VECTOR ObjectBase::GetModelFramePos(const char* frameName)
{
	return MV1GetFramePosition(m_modelHandle, MV1SearchFrame(m_modelHandle, frameName));
}
