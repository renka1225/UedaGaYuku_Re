#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 攻撃状態
/// </summary>
class PlayerStateAttack : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateAttack(std::shared_ptr<Player> player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateAttack() {}

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
	virtual void Update(const Input& input, const Camera& camera, Stage& stage) override;

	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kAttack; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override { return "攻撃中"; }
#endif

private:
	bool m_isAttackEnd; // 攻撃が終わったかどうか
};

