#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"

class Input;
class Camera;
class Stage;

/// <summary>
/// �v���C���[
/// </summary>
class Player : public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();
	virtual void Init();
	virtual void Update(const Input& input, const Camera& camera, Stage& stage);
	virtual void Draw();

	void Move(const VECTOR& moveVec);	// �ړ�����
	void UpdateAngle();					// �v���C���[�̊p�x���X�V

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// state�p�^�[��
	DINPUT_JOYSTATE AnalogInput; 	// �A�i���O�p�b�h���擾
};

