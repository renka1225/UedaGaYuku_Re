#pragma once
#include "DxLib.h"
#include <memory>
#include <string>

class EnemyBase;
class EnemyAI;
class Stage;

/// <summary>
/// 敵の状態を管理する基底クラス
/// </summary>
class EnemyStateBase : public std::enable_shared_from_this<EnemyStateBase>
{
public:
	// 敵の状態
	enum class EnemyStateKind
	{
		kIdle,	 // 待機
		kWalk,	 // 歩き
		kRun,	 // 走り
		kPunch,  // パンチ攻撃
		kKick,	 // キック攻撃
		kAvoid,	 // 回避
		kGuard,	 // ガード
		kDamage, // 攻撃を受ける
		kDeath,	 // 死亡
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵参照</param>
	/// <param name="pEnemyAI">敵AI参照</param>
	EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<EnemyAI> pEnemyAI);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateBase() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pStage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照/param>
	virtual void Update(Stage& pStage, Player& pPlayer);
	
	/// <summary>
	/// 現在の状態を取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual EnemyStateKind GetKind() = 0;
	
#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() = 0;
#endif

protected:
	/// <summary>
	/// 待機状態に変更する
	/// </summary>
	void ChangeStateIdle();

	/// <summary>
	/// 歩き状態に変更する
	/// </summary>
	void ChangeStateWalk();

	/// <summary>
	/// 走り状態に変更する
	/// </summary>
	void ChangeStateRun();

	/// <summary>
	/// パンチ攻撃状態に変更する
	/// </summary>
	void ChangeStatePunch();

	/// <summary>
	/// キック攻撃状態に変更する
	/// </summary>
	void ChangeStateKick();

	/// <summary>
	/// ガード状態に変更する
	/// </summary>
	void ChangeStateGuard();

	/// <summary>
	/// 回避状態に変更する
	/// </summary>
	void ChangeStateAvoid();

	/// <summary>
	/// ダメージ状態に変更する
	/// </summary>
	void ChangeStateDamage();

	/// <summary>
	/// 死亡状態に変更する
	/// </summary>
	void ChangeStateDeath();

private:
	/// <summary>
	// ステートを更新する
	/// </summary>
	void UpdateState();

		
public:
	std::shared_ptr<EnemyStateBase> m_nextState; // 次のStateを保存する

protected:
	std::shared_ptr<EnemyBase> m_pEnemy; // 敵のポインタ
	std::shared_ptr<EnemyAI> m_pEnemyAI; // AIのポインタ
	VECTOR m_upMoveVec;		// 上ボタンを入力をしたときの移動方向ベクトル
	VECTOR m_leftMoveVec;	// 左ボタンを入力をしたときの移動方向ベクトル
	VECTOR m_moveVec;		// 移動ベクトル
	float m_animEndTime;	// アニメーションが終わるまでの時間
};

