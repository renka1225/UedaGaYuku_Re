#pragma once

class Input;
class Player;
class Stage;

/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Update(Input& input, const Player& player, const Stage& stage);

	VECTOR GetAngle() const { return m_target; } // �J�����̊p�x���擾
	VECTOR GetPos() const { return m_pos; }		 // �J�����ʒu���擾
	float GetAngleH() const{ return m_angleH; }	 // �J�����̐����p�x���擾

private:
	void FixCameraPos();					 // �J�����ʒu�␳
	void CheckCameraCol(const Stage& stage); // �J�����̓����蔻����`�F�b�N����

private:
	VECTOR m_pos;					// �J�������W
	VECTOR m_target;				// �����_���W
	float m_angleH;					// �����p�x
	float m_angleV;					// �����p�x
	MATRIX m_rotY;					// Y����]�s��
	MATRIX m_rotZ;					// Z����]�s��
	DINPUT_JOYSTATE m_analogInput; 	// �A�i���O�p�b�h���擾
	int m_lightHandle;				// ���C�g�n���h��
};

