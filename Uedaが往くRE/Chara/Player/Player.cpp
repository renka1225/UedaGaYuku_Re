#include "DxLib.h"
#include "Input.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1");	// ���f���̃t�@�C����
	const VECTOR kInitPos = VGet(3000.0f, 12.0f, 3000.0f);			// �����ʒu
	constexpr float kScale = 0.05f;	// �g�嗦
	constexpr float kMove = 1.0f;	// �ړ���
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	CharacterBase::~CharacterBase();
}

/// <summary>
/// ������
/// </summary>
void Player::Init()
{
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	Move(input);
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="moveVec">�ړ��x�N�g���Q��</param>
/// <param name="stage">�X�e�[�W���Q��</param>
void Player::Move(const Input& input)
{
	if (input.IsPressing("right"))
	{
		m_pos.x += kMove;
	}
	if (input.IsPressing("left"))
	{
		m_pos.x -= kMove;
	}
	if (input.IsPressing("up"))
	{
		m_pos.z += kMove;
	}
	if (input.IsPressing("down"))
	{
		m_pos.z -= kMove;
	}

	MV1SetPosition(m_modelHandle, m_pos);
}
