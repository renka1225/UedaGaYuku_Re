#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 攻撃状態
/// </summary>
class PlayerStateAttack : public PlayerStateBase
{
public:
	// プレイヤーの攻撃種類
	enum class PlayerAttackType
	{
		kPunch1,		// パンチ1コンボ目
		kPunch2,		// パンチ2コンボ目
		kPunch3,		// パンチ3コンボ目
		kKick,			// キック
		kHeatKick,		// ヒートアクション
		kSpecialAttack	// ヒートアクション2
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateAttack(const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateAttack() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="attackName">入力された攻撃名</param>
	void Init(std::string attackName);

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
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kAttack; }

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() override;
#endif

private:
	/// <summary>
	/// 攻撃処理の更新
	/// </summary>
	void UpdateAttack(Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy);

	/// <summary>
	/// アニメーションの終了時間を取得する
	/// </summary>
	float GetAnimEndTime();

	/// <summary>
	/// プレイヤーの現在の攻撃力を取得する
	/// </summary>
	/// <returns>攻撃力</returns>
	float GetAttackPower();

	/// <summary>
	/// プレイヤーの追加ゲージ量を取得する
	/// </summary>
	/// <returns>追加ゲージ量</returns>
	float GetAddGauge();

private:
	std::string m_attackKind;	// 入力された攻撃の種類
	bool m_isAttackEnd;			// 攻撃が終わったかどうか
};

