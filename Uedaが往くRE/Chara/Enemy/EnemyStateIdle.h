#pragma once
#include "EnemyStateBase.h"

class Stage;

/// <summary>
/// 敵の待機状態
/// </summary>
class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {};
	void Init();
	virtual void Update(Stage& stage);
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kIdle; };

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "待機"; }
#endif
};

