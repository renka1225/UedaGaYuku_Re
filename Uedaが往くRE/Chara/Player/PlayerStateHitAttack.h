#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 攻撃を受けた状態
/// </summary>
class PlayerStateHitAttack : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateHitAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateHitAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kDamage; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "攻撃を受ける"; }
#endif
};

