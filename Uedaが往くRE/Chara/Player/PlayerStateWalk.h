#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// 歩き状態
/// </summary>
class PlayerStateWalk : public PlayerStateBase
{
public:
	PlayerStateWalk(std::shared_ptr<Player> player) : PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage) override;
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kWalk; }

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "歩き"; }
#endif
};

