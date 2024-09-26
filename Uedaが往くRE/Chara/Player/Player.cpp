#include "DxLib.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1"); // ���f���̃t�@�C����
	const VECTOR kInitPos = VGet(0.0f, 0.0f, 0.0f);				  // �����ʒu
	constexpr float kScale = 0.1f;
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
void Player::Update()
{
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();
}
