﻿#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 攻撃状態
/// </summary>
class EnemyStateAttack : public EnemyStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyStateAttack(std::shared_ptr<EnemyBase> pEnemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pStage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照/param>
	virtual void Update(Stage& pStage, Player& pPlayer) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateBase::EnemyStateKind GetKind() override;

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override;
#endif

	/// <summary>
	/// 敵の現在の攻撃力を取得する
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <returns>攻撃力</returns>
	float GetAttackPower(Player& pPlayer);

	/// <summary>
	/// 攻撃の種類をセットする
	/// </summary>
	/// <param name="attackName">入力された攻撃名</param>
	void SetAttackKind(std::string attackName) { m_attackKind = attackName; }

private:
	std::string m_attackKind;	// 入力された攻撃の種類
	float m_attackEndTime;		// 攻撃が終わるまでの時間
	bool m_isAttackEnd;			// 攻撃が終わったかどうか
};

