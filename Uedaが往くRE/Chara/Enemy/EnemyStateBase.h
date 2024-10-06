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
		kIdle,	// 待機
		kWalk,	// 歩き
		kRun,	// 走り
	};

	EnemyStateBase(std::shared_ptr<EnemyBase> pEnemy);
	virtual void Update(Stage& stage) = 0;
	// 現在の状態を取得
	virtual EnemyStateKind GetKind() = 0;

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() = 0;
#endif

public:
	std::shared_ptr<EnemyStateBase> m_nextState; // 次のStateを保存する

protected:
	std::shared_ptr<EnemyBase> m_pEnemy; // 敵のポインタ
};

