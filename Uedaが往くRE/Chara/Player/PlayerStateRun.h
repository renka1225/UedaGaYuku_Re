#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// ‘–‚èó‘Ô
/// </summary>
class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun(std::shared_ptr<Player> player) : PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera);
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kRun; }

#ifdef _DEBUG
	// Œ»İ‚Ìó‘Ô–¼‚ğæ“¾
	virtual std::string GetStateName() override { return "‘–‚è"; }
#endif
};

