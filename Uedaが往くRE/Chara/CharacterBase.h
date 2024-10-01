#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
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

	// アニメーション情報
	struct AnimInfo
	{
		int number = 0;
		float loopFrame = 0.0f;
		float endFrame = 0.0f;
		float playSpeed = 0.0f;
	};

public:
	CharacterBase();
	~CharacterBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void ChangeAnim(std::string animName);				// アニメーションを変更
	void UpdateAnim();									// アニメーションを更新

	VECTOR GetPos() const { return m_pos; }				// 現在位置取得
	Status GetStatus() const { return m_status; }		// ステータス取得

protected:
	std::map<std::string, AnimInfo> m_animData;		// アニメーションのデータ
	Status m_status;	  // ステータス
	VECTOR m_pos;		  // 現在位置
	VECTOR m_moveDir;	  // 向くべき方向のベクトル
	float m_angle;		  // 向いている方向の角度
	float m_hp;			  // HP
	int m_modelHandle;	  // キャラクターのモデル

	int m_currentPlayAnim;		// 現在のアニメーション
	int m_prevPlayAnim;			// 前に再生していたアニメーション
	float m_animBlendRate;		// 現在と過去のアニメーションのブレンド率
	float m_currentAnimTime;	// 現在のアニメーション再生時間
	float m_prevAnimTime;		// 前のアニメーション再生時間
	float m_totalAnimTime;		// アニメーションの総再生時間
	float m_animPlaySpeed; 		// アニメーションの再生速度
	float m_animLoopStartTime;	// アニメーションがループが始まる時間
	float m_animLoopEndTime;	// アニメーションがループが終わる時間
	bool m_isLoopAnim;			// アニメーションをループさせるか
};

