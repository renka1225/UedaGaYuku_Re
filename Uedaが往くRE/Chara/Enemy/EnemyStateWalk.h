#pragma once
#include "EnemyStateBase.h"

/// <summary>
/// 歩き状態
/// </summary>
class EnemyStateWalk : public EnemyStateBase
{
public:
	EnemyStateWalk(std::shared_ptr<EnemyBase> pEnemy) : EnemyStateBase(pEnemy) {};
	void Init();
	virtual void Update(Stage& stage) override;
	virtual EnemyStateKind GetKind() override { return EnemyStateKind::kWalk; }

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "歩き"; }
#endif
};

