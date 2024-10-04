#pragma once
#include "EnemyBase.h"

/// <summary>
/// 敵ID01のクラス
/// </summary>
class Enemy01 : public EnemyBase
{
public:
	Enemy01();
	virtual ~Enemy01();
	virtual void Init() override;
	virtual void Update(Stage& stage) override;
	virtual void Draw() override;
};

