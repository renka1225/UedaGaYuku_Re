#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// ‘Ò‹@ó‘Ô
/// </summary>
class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player): PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera);
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kIdle; }

#ifdef _DEBUG
	// Œ»İ‚Ìó‘Ô–¼‚ğæ“¾
	virtual std::string GetStateName() override { return "‘Ò‹@"; }
#endif
};

