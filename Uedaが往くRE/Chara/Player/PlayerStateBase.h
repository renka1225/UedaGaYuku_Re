#pragma once
#include <memory>
#include <string>

class Input;
class Camera;
class Player;

/// <summary>
/// �v���C���[�̏�Ԃ��Ǘ�������N���X
/// </summary>
class PlayerStateBase : public std::enable_shared_from_this<PlayerStateBase>
{
public:
	// �v���C���[�̏��
	enum class PlayerStateKind
	{
		kIdle,	// �ҋ@
		kWalk,	// ����
		kRun	// ����
	};

	PlayerStateBase(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	virtual void Update(const Input& input, const Camera& camera) = 0;
	// ���݂̏�Ԃ��擾
	virtual PlayerStateKind GetKind() = 0;

#ifdef _DEBUG
	// ���݂̏�Ԗ����擾
	virtual std::string GetStateName() = 0;
#endif

public:
	std::shared_ptr<PlayerStateBase> m_nextState;	// ����State��ۑ�����

protected:
	std::shared_ptr<Player> m_pPlayer;				// �v���C���[�̃|�C���^
};