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

protected:
	VECTOR m_pos;		// ���݈ʒu
	int m_modelHandle;	// �L�����N�^�[�̃��f��
};

