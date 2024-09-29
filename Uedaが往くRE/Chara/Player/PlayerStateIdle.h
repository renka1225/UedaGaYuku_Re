#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// �ҋ@���
/// </summary>
class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player): PlayerStateBase(player) {};
	void Init();
	virtual void Update(const Input& input, const Camera& camera);
	virtual PlayerStateKind GetKind() override { return PlayerStateKind::kIdle; }

#ifdef _DEBUG
	// ���݂̏�Ԗ����擾
	virtual std::string GetStateName() override { return "�ҋ@"; }
#endif
};

