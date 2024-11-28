#include "Vec2.h"
#include "Game.h"
#include "Font.h"
#include "DebugDraw.h"

namespace
{
	constexpr int kTextInterval = 60;	// 文字列の表示間隔
	const Vec2 kAnimFramePos = { 400.0f, 900.0f };	// アニメーションフレーム表示位置
}

void DebugDraw::DrawPlayerInfo(const VECTOR pos, float hp, CharacterBase::Status status, std::string state, bool isGrabWeapon)
{
	DrawFormatString(0, 180, Color::kColorW, "プレイヤー座標(X:%.2f, Y:%.2f, Z:%.2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 200, Color::kColorW, "プレイヤーHP:%.2f", hp);
	DrawFormatString(0, 220, Color::kColorW, "プレイヤー最大HP:%.2f", status.maxHp);
	DrawFormatString(0, 240, Color::kColorW, "プレイヤー最大ゲージ量:%.2f", status.maxGauge);
	DrawFormatString(0, 260, Color::kColorW, "プレイヤー状態:%s", state.c_str());

	if (isGrabWeapon)
	{
		DrawString(0, 280, "武器掴み中", Color::kColorW);
	}
}

void DebugDraw::DrawEnemyInfo(const VECTOR pos, float hp, int index, std::string state)
{
	DrawFormatString(0, 240 + index * kTextInterval, Color::kColorW, "敵座標(X:%.2f, Y:%.2f, Z:%.2f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 260 + index * kTextInterval, Color::kColorW, "敵HP:%.2f", hp);
	DrawFormatString(0, 280 + index * kTextInterval, Color::kColorW, "敵状態:%s", state.c_str());
}

void DebugDraw::DrawWeaponInfo(std::string name, std::string tag, VECTOR pos, VECTOR rot, VECTOR scale, int durability)
{
	
	//DrawFormatString(0, 440, 0xffffff, "name:%s, tag:%s, pos(%.2f:%.2f:%.2f), rot(%.2f:%.2f:%.2f), scale(%.2f:%.2f:%.2f), 耐久力:%d\n",
		//name.c_str(), tag.c_str(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, scale.x, scale.y, scale.z, durability);
}

void DebugDraw::DrawBodyCol(CharacterBase::ColData& colData)
{
	//DrawFormatString(0, 380, 0xffffff, "当たり判定始点(X:%.2f,Y:%.2f,Z:%.2f)", colData.bodyUpdateStartPos.x, colData.bodyUpdateStartPos.y, colData.bodyUpdateStartPos.z);
	//DrawFormatString(0, 400, 0xffffff, "当たり判定終点(X:%.2f,Y:%.2f,Z:%.2f)", colData.bodyUpdateEndPos.x, colData.bodyUpdateEndPos.y, colData.bodyUpdateEndPos.z);

	DrawCapsule3D(colData.bodyUpdateStartPos, colData.bodyUpdateEndPos, colData.bodyRadius, 1, Color::kColorP, Color::kColorBK, false);
}

void DebugDraw::DrawArmCol(CharacterBase::ColData& colData)
{
	// フレーム部分を点で表示する
	DrawModelFrameCircle(colData.leftShoulderPos);
	DrawModelFrameCircle(colData.leftForeArmPos);
	DrawModelFrameCircle(colData.leftHandPos);
	DrawModelFrameCircle(colData.rightShoulderPos);
	DrawModelFrameCircle(colData.rightForeArmPos);
	DrawModelFrameCircle(colData.rightHandPos);

	// 当たり判定カプセルを表示
	DrawModelFrameCapsule3D(colData.leftShoulderPos, colData.leftForeArmPos, colData.armRadius, Color::kColorLB);
	DrawModelFrameCapsule3D(colData.leftForeArmPos, colData.leftHandPos, colData.armRadius, Color::kColorLB);
	DrawModelFrameCapsule3D(colData.rightShoulderPos, colData.rightForeArmPos, colData.armRadius, Color::kColorLB);
	DrawModelFrameCapsule3D(colData.rightForeArmPos, colData.rightHandPos, colData.armRadius, Color::kColorLB);
}

void DebugDraw::DrawLegCol(CharacterBase::ColData& colData)
{
	// フレーム部分を点で表示する
	DrawModelFrameCircle(colData.leftUpLegPos);
	DrawModelFrameCircle(colData.leftLegPos);
	DrawModelFrameCircle(colData.leftHandPos);
	DrawModelFrameCircle(colData.leftFootPos);
	DrawModelFrameCircle(colData.leftEndPos);
	DrawModelFrameCircle(colData.rightUpLegPos);
	DrawModelFrameCircle(colData.rightLegPos);
	DrawModelFrameCircle(colData.rightFootPos);
	DrawModelFrameCircle(colData.rightEndPos);

	// 当たり判定カプセルを表示
	DrawModelFrameCapsule3D(colData.leftUpLegPos, colData.leftLegPos, colData.legRadius, Color::kColorY);
	DrawModelFrameCapsule3D(colData.leftLegPos, colData.leftFootPos, colData.legRadius, Color::kColorY);
	DrawModelFrameCapsule3D(colData.leftFootPos, colData.leftEndPos, colData.legRadius, Color::kColorY);
	DrawModelFrameCapsule3D(colData.rightUpLegPos, colData.rightLegPos, colData.legRadius, Color::kColorY);
	DrawModelFrameCapsule3D(colData.rightLegPos, colData.rightFootPos, colData.legRadius, Color::kColorY);
	DrawModelFrameCapsule3D(colData.rightFootPos, colData.rightEndPos, colData.legRadius, Color::kColorY);
}

void DebugDraw::DrawWeaponCol(VECTOR startPos, VECTOR endPos, float r)
{
	DrawCapsule3D(startPos, endPos, r, 1, Color::kColorR, Color::kColorBK, false);
}

void DebugDraw::DrawItemCol(VECTOR centerPos, float r)
{
	DrawSphere3D(centerPos, r, 1, Color::kColorG, Color::kColorBK, false);
}

void DebugDraw::DrawModelFrameCircle(VECTOR pos)
{
	DrawCircleAA(ConvWorldPosToScreenPos(pos).x, ConvWorldPosToScreenPos(pos).y, 5, Color::kColorR, true);
}

void DebugDraw::DrawModelFrameCapsule3D(VECTOR startPos, VECTOR endPos, float radius, int capsuleColor)
{
	DrawCapsule3D(startPos, endPos, radius, 1, capsuleColor, Color::kColorBK, false);
}

void DebugDraw::DrawAnimFrame(float animTotalTime)
{
	DrawFormatStringFToHandle(kAnimFramePos.x, kAnimFramePos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kDebug_animFrame)], "フレーム数:%.2f\n", animTotalTime);
}

void DebugDraw::DrawInputCommand()
{
}
