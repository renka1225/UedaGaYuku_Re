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
	/// <param name="pStage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤーポインタ/param>
	virtual void Update(Stage& pStage, Player& pPlayer) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kDamage; }

	/// <summary>
	/// 無敵時間をセットする
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void SetInvincibleTime(Player& pPlayer);

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "ダメージ"; }
#endif
};

