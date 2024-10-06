#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 走り状態
/// </summary>
class EnemyStateRun : public EnemyStateBase
{
public:
	EnemyStateRun(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {};
	void Init();
	virtual void Update(Stage& stage) override;
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kRun; }

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "走り"; }
#endif
};

