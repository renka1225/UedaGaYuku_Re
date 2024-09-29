#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// •à‚«ó‘Ô
/// </summary>
class PlayerStateWalk : public PlayerStateBase
{
public:
	PlayerStateWalk(std::shared_ptr<Player> player) : PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera);
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kWalk; }

#ifdef _DEBUG
	// Œ»İ‚Ìó‘Ô–¼‚ğæ“¾
	virtual std::string GetStateName() override { return "•à‚«"; }
#endif
};

