#include "Game.h"
#include "DebugDraw.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DebugDraw::DebugDraw()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DebugDraw::~DebugDraw()
{
}

/// <summary>
/// ������
/// </summary>
void DebugDraw::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
void DebugDraw::Update()
{
}

/// <summary>
/// �f�o�b�O���`��
/// </summary>
/// <param name="pos">���W</param>
void DebugDraw::DrawInfo(const VECTOR pos)
{
	DrawFormatString(0, 0, TextColor::kColorW, "���W(X:%f, Y:%f, Z:%f)", pos.x, pos.y, pos.z);
}
