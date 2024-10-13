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
	EnemyStateAttack(std::shared_ptr<EnemyBase> enemy) : EnemyStateBase(enemy){};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(Stage& stage) override;

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
};

