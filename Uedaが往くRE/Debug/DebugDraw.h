#pragma once
#include "CharacterBase.h"
#include "DxLib.h"
#include <string>

/// <summary>
/// デバッグ表示
/// </summary>
class DebugDraw
{
public:
	DebugDraw() {}

	~DebugDraw() {}

	/// <summary>
	/// プレイヤー情報を描画
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="hp">現在のHP</param>
	/// <param name="status">ステータス情報</param>
	/// <param name="state">現在の状態/param>
	/// <param name="isGrabWeapon">武器掴み中か/param>
	void DrawPlayerInfo(const VECTOR pos, float hp, CharacterBase::Status status, std::string state, bool isGrabWeapon);

	/// <summary>
	/// 敵情報を描画
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="hp">現在のHP</param>
	/// <param name="index">敵の番号</param>
	/// <param name="state">現在の状態</param>
	void DrawEnemyInfo(const VECTOR pos, float hp, int index, std::string state);

	/// <summary>
	/// 武器情報を描画
	/// </summary>
	/// <param name="name">武器名</param>
	/// <param name="tag">武器の種類</param>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転率</param>
	/// <param name="scale">サイズ</param>
	/// <param name="durability">耐久力</param>
	void DrawWeaponInfo(std::string name, std::string tag, VECTOR pos, VECTOR rot, VECTOR scale, int durability);

	/// <summary>
	/// 全身の当たり判定を描画
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void DrawBodyCol(CharacterBase::ColData& colData);

	/// <summary>
	/// 腕の当たり判定を描画
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void DrawArmCol(CharacterBase::ColData& colData);

	/// <summary>
	///  脚の当たり判定を描画
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	void DrawLegCol(CharacterBase::ColData& colData);

	/// <summary>
	/// 武器の当たり判定を描画
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="r">半径</param>
	void DrawWeaponCol(VECTOR startPos, VECTOR endPos, float r);

	/// <summary>
	/// アイテムの当たり判定を描画
	/// </summary>
	/// <param name="centerPos">中心点</param>
	/// <param name="r">半径</param>
	void DrawItemCol(VECTOR centerPos, float r);

	/// <summary>
	/// モデルのフレーム部分を円で描画する
	/// </summary>
	/// <param name="pos">表示位置</param>
	void DrawModelFrameCircle(VECTOR pos);

	/// <summary>
	/// 当たり判定をカプセルで表示する
	/// </summary>
	/// <param name="startPos">始点位置</param>
	/// <param name="endPos">終点位置</param>
	/// <param name="radius">カプセルの半径</param>
	/// <param name="capsuleColor">カプセルの色</param>
	void DrawModelFrameCapsule3D(VECTOR startPos, VECTOR endPos, float radius, int capsuleColor);

	/// <summary>
	/// アニメーションのフレーム数を表示する
	/// </summary>
	/// <param name="animTotalTime">アニメーションの総再生時間</param>
	/// <param name="animName">アニメーション名</param>
	/// <param name="animData">アニメーションのデータ</param>
	void DrawAnimFrame(float animTotalTime, std::string animName,std::map<std::string, CharacterBase::AnimInfo> animData);

	/// <summary>
	/// イベント用の当たり判定を描画
	/// </summary>
	/// <param name="centerPos">表示位置</param>
	/// <param name="r">半径</param>
	void DrawEventCol(VECTOR centerPos, float r);
};

