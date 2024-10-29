#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 回避処理
/// </summary>
class EnemyStateAvoid : public EnemyStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵参照</param>
	EnemyStateAvoid(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyStateAvoid() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤーポインタ/param>
	virtual void Update(Stage& stage, Player& pPlayer);

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kAvoid; };

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "回避"; }
#endif
};

