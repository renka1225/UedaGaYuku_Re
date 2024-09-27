#pragma once
#include "DxLib.h"

/// <summary>
/// �L�����N�^�[�̊��N���X
/// </summary>
class CharacterBase
{
public:
	CharacterBase();
	~CharacterBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	VECTOR GetPos() const { return m_pos; } // ���݈ʒu�擾

public:
	// �X�e�[�^�X�\����
	struct Status
	{
		float maxHp;		// �ő�HP
		float walkSpeed;	// �������x
		float runSpeed;		// ���葬�x
	};

protected:
	Status m_status;	// �X�e�[�^�X
	float m_hp;			// HP
	VECTOR m_pos;		// ���݈ʒu
	int m_modelHandle;	// �L�����N�^�[�̃��f��
};

