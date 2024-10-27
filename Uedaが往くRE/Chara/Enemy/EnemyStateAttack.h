#pragma once
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
	EnemyStateAttack(std::shared_ptr<EnemyBase> enemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="attackName">入力された攻撃名</param>
	void Init(std::string attackName);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照/param>
	virtual void Update(Stage& stage, Player& pPlayer) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kAttack; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "攻撃中"; }
#endif

private:
	std::string m_attackKind;	// 入力された攻撃の種類
	float m_attackEndTime;		// 攻撃が終わるまでの時間
	bool m_isAttackEnd;			// 攻撃が終わったかどうか
};

