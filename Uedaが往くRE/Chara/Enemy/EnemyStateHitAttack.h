#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 攻撃を受けた状態
/// </summary>
class EnemyStateHitAttack : public EnemyStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyStateHitAttack(std::shared_ptr<EnemyBase> enemy) : EnemyStateBase(enemy) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateHitAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤーポインタ/param>
	virtual void Update(Stage& stage, Player& pPlayer) override;

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <returns>ダメージ量</returns>
	//virtual int OnDamage() override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kDamage; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "ダメージ"; }
#endif
};

