#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 走り状態
/// </summary>
class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun(std::shared_ptr<Player> player) : PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage) override;
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kRun; }

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "走り"; }
#endif
};

