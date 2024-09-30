#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include <map>

/// <summary>
/// �L�����N�^�[�̊��N���X
/// </summary>
class CharacterBase
{
public:
	// �X�e�[�^�X�\����
	struct Status
	{
		float maxHp;		// �ő�HP
		float walkSpeed;	// �������x
		float runSpeed;		// ���葬�x
	};

	// �A�j���[�V�������
	struct AnimInfo
	{
		int number = 0;
		int loopFrame = 0;
		int endFrame = 0;
		float playSpeed = 0.0f;
	};

public:
	CharacterBase();
	~CharacterBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void ChangeAnim(std::string animName);			// �A�j���[�V������ύX
	void UpdateAnim();								// �A�j���[�V�������X�V

	VECTOR GetPos() const { return m_pos; }			// ���݈ʒu�擾
	Status GetStatus() const { return m_status; }	// �X�e�[�^�X�擾

protected:
	AnimInfo m_animData; // �A�j���[�V�����̃f�[�^
	Status m_status;	 // �X�e�[�^�X
	VECTOR m_pos;		 // ���݈ʒu
	VECTOR m_moveDir;	 // �����ׂ������̃x�N�g��
	float m_angle;		 // �����Ă�������̊p�x
	float m_hp;			 // HP
	int m_modelHandle;	 // �L�����N�^�[�̃��f��

	int m_currentPlayAnim;		// ���݂̃A�j���[�V����
	int m_prevPlayAnim;			// �O�ɍĐ����Ă����A�j���[�V����
	float m_currentAnimTime;	// ���݂̃A�j���[�V�����Đ�����
	float m_prevAnimTime;		// �O�̃A�j���[�V�����Đ�����
	float m_totalAnimTime;		// �A�j���[�V�����̑��Đ�����
	float m_animPlaySpeed; 		// �A�j���[�V�����̍Đ����x
	float m_animBlendRate;		// ���݂Ɖߋ��̃A�j���[�V�����̃u�����h��
};

