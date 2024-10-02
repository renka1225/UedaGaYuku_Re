#include "DxLib.h"
#include "DebugDraw.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Camera.h"
#include "Stage.h"
#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1");	// ���f���̃t�@�C����
	const VECTOR kInitPos = VGet(7425.0, 40.0f, 5190.0f);			// �����ʒu
	constexpr float kScale = 0.14f;									// �g�嗦
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);

	// �X�e�[�^�X��ǂݍ���
	LoadCsv::GetInstance().LoadStatus(m_status, "player");
	m_hp = m_status.maxHp;
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
	CharacterBase::Init();

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	m_pState = std::make_shared<PlayerStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;
	
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	// �O�̃t���[���ƈႤstate�̏ꍇ
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// state��ύX����
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	m_pState->Update(input, camera); 	// state�̍X�V
	UpdateAngle();	// �v���C���[�̌������X�V
	UpdateAnim();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName()); // �v���C���[�̏���`��
#endif
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="moveVec">�ړ���</param>
void Player::Move(const VECTOR& moveVec)
{
	m_pos = VAdd(m_pos, moveVec);
	m_moveDir = VNorm(moveVec);
	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// �v���C���[�̊p�x���X�V
/// </summary>
void Player::UpdateAngle()
{
	m_angle = atan2f(m_moveDir.x, m_moveDir.z);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
