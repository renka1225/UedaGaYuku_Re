#pragma once
#include "PlayerStateBase.h"
#include <vector>

/// <summary>
/// 死亡時の処理
/// </summary>
class PlayerStateDeath : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	PlayerStateDeath(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateDeath() {}

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
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kDeath; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "死亡"; }
#endif
};

