﻿#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 攻撃状態
/// </summary>
class PlayerStateAttack : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateAttack(std::shared_ptr<Player> player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="attackName">入力された攻撃名</param>
	void Init(std::string attackName);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kAttack; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override;
#endif

private:
	std::string m_attackKind;	// 入力された攻撃の種類
	float m_attackEndTime;		// 攻撃が終わるまでの時間
	bool m_isAttackEnd;			// 攻撃が終わったかどうか
};

