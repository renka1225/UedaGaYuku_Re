#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// ������
/// </summary>
class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun(std::shared_ptr<Player> player) : PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera);
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kRun; }

#ifdef _DEBUG
	// ���݂̏�Ԗ����擾
	virtual std::string GetStateName() override { return "����"; }
#endif
};

