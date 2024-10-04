#pragma once
#include "CharacterBase.h"
#include "EnemyStateBase.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	EnemyBase();
	virtual ~EnemyBase();
	virtual void Init();
	virtual void Update(Stage& stage);
	virtual void Draw();

protected:
	std::shared_ptr<EnemyStateBase> m_pState;	// stateパターン
};

