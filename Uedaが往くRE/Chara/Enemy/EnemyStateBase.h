#pragma once
#include "DxLib.h"
#include <memory>
#include <string>

class EnemyBase;
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
		kAttack, // 攻撃
		kDamage, // 攻撃を受ける
		kDeath,	 // 死亡
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵参照</param>
	EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyStateBase() {};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	/// <param name="pPlayer">プレイヤー参照/param>
	virtual void Update(Stage& stage, Player& pPlayer);
	
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

public:
	std::shared_ptr<EnemyStateBase> m_nextState; // 次のStateを保存する

protected:
	std::shared_ptr<EnemyBase> m_pEnemy; // 敵のポインタ
};

