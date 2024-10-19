#include "Game.h"
#include "DebugDraw.h"

void DebugDraw::DrawPlayerInfo(const VECTOR pos, float hp, std::string state)
{
	DrawFormatString(0, 20, Color::kColorW, "プレイヤー座標(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 40, Color::kColorW, "プレイヤーHP:%2f", hp);
	DrawFormatString(0, 60, Color::kColorW, "プレイヤー状態:%s", state.c_str());
}

void DebugDraw::DrawEnemyInfo(const VECTOR pos, float hp, std::string state)
{
	DrawFormatString(0, 80, Color::kColorW, "敵座標(X:%2f, Y:%2f, Z:%2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 100, Color::kColorW, "敵HP:%2f", hp);
	DrawFormatString(0, 120, Color::kColorW, "敵状態:%s", state.c_str());
}

void DebugDraw::DrawWeaponInfo(std::string name, std::string tag, VECTOR pos, VECTOR rot, VECTOR scale, int durability)
{
	
	DrawFormatString(0, 300, 0xffffff, "name:%s, tag:%s, pos(%2.2f:%2.2f:%2.2f), rot(%2.2f:%2.2f:%2.2f), scale(%2.2f:%2.2f:%2.2f), 耐久力:%d\n",
		name.c_str(), tag.c_str(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, scale.x, scale.y, scale.z, durability);
}

void DebugDraw::DrawBodyCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorP, Color::kColorBK, false);
}

void DebugDraw::DrawAimCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorLB, Color::kColorBK, false);
}

void DebugDraw::DrawLegCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorY, Color::kColorBK, false);
}

void DebugDraw::DrawWeaponCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorY, Color::kColorG, false);
}
