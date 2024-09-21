#pragma once
#include "CharacterBase.h"

/// <summary>
/// ƒvƒŒƒCƒ„[
/// </summary>
class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();
	virtual void Init();
	virtual void Update();
	virtual void Draw();
};

