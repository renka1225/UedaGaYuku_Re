#include "DxLib.h"
#include "DebugDraw.h"
#include "Input.h"
#include "CsvLoad.h"
#include "Camera.h"
#include "Stage.h"
#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	const char* kModelFileName = ("data/model/chara/player.mv1");	// ���f���̃t�@�C����
	const VECTOR kInitPos = VGet(2020.0, 12.0f, 1800.0f);			// �����ʒu
	constexpr float kScale = 0.05f;	// �g�嗦
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);

	// �X�e�[�^�X��ǂݍ���
	CsvLoad::GetInstance().LoadStatus(m_status, "player");
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

	// state�̍X�V
	m_pState->Update(input, camera);
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
	MV1SetPosition(m_modelHandle, m_pos);
}
