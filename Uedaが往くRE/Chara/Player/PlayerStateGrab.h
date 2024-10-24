#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 掴み状態
/// </summary>
class PlayerStateGrab : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	PlayerStateGrab(std::shared_ptr<Player> player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateGrab() {}

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
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kGrab; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "掴み"; }
#endif

private:
	float m_animTime;	// アニメーションが終わるまでの時間
	bool m_isAnimEnd;	// アニメーションが終わったかどうか
};

