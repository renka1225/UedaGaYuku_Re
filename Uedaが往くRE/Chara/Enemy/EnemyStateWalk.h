#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 歩き状態
/// </summary>
class EnemyStateWalk : public EnemyStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵参照</param>
	EnemyStateWalk(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyStateWalk() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

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
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kWalk; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "歩き"; }
#endif
};

