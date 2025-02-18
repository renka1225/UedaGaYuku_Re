#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 移動状態
/// </summary>
class EnemyStateMove : public EnemyStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	EnemyStateMove(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateMove() {}

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
	/// アニメーションの種類をセットする
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	void SetAnimKind(std::string animName) { m_animKind = animName; }

private:
	std::string m_animKind; // 再生するアニメーション名
};

