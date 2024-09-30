#pragma once
#include "DxLib.h"
#include <map>

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class CharacterBase
{
public:
	// ステータス構造体
	struct Status
	{
		float maxHp;		// 最大HP
		float walkSpeed;	// 歩き速度
		float runSpeed;		// 走り速度
	};

public:
	CharacterBase();
	~CharacterBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void ChangeAnim(std::string animName);			// アニメーションを変更する

	VECTOR GetPos() const { return m_pos; }			// 現在位置取得
	Status GetStatus() const { return m_status; }	// ステータス取得

protected:
	// アニメーション情報
	struct AnimInfo
	{
		int number = 0;
		int loopFrame = 0;
		int endFrame = 0;
		float playSpeed = 0.0f;
	};

protected:
	std::map<std::string, AnimInfo> m_animData; // アニメーションのデータ

	Status m_status;	// ステータス
	VECTOR m_pos;		// 現在位置
	VECTOR m_moveDir;	// 向くべき方向のベクトル
	float m_angle;		// 向いている方向の角度
	float m_hp;			// HP
	int m_modelHandle;	// キャラクターのモデル

	int m_playAnim;			// 再生中のアニメーション
	float m_animPlaySpeed; 	// アニメーションの再生速度
	float m_totalAnimTime; 	// アニメーションの総再生時間
	float m_animTime;		// アニメーションの再生時間
};

