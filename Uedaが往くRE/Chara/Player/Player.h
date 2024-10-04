#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"

class Input;
class Camera;
class Stage;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();
	virtual void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage);
	virtual void Draw();

	void UpdateAngle();					// プレイヤーの角度を更新

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// stateパターン
};

