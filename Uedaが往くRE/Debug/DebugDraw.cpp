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
/// �v���C���[����`��
/// </summary>
/// <param name="pos">���W</param>
/// <param name="hp">���݂�HP</param>
void DebugDraw::DrawPlayerInfo(const VECTOR pos, int hp, std::string state)
{
	DrawFormatString(0, 20, TextColor::kColorW, "���W(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 40, TextColor::kColorW, "HP:%2f", hp);
	DrawFormatString(0, 60, TextColor::kColorW, "���:%s", state.c_str());
}