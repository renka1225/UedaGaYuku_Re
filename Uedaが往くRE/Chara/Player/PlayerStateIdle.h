#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 待機状態
/// </summary>
class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player): PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage) override;
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kIdle; }

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "待機"; }
#endif
};

