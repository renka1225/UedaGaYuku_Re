#pragma once
#include "CharacterBase.h"

class Input;
class Camera;
class Stage;

/// <summary>
/// ƒvƒŒƒCƒ„[
/// </summary>
class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();
	virtual void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage);
	virtual void Draw();

private:
	void Move(const Input& input);
};

