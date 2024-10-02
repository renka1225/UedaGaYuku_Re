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
	const char* kModelFileName = ("data/model/chara/player.mv1");	// モデルのファイル名
	const VECTOR kInitPos = VGet(7425.0, 40.0f, 5190.0f);			// 初期位置
	constexpr float kScale = 0.14f;									// 拡大率
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, "player");
	m_hp = m_status.maxHp;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	CharacterBase::~CharacterBase();
}

/// <summary>
/// 初期化
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
/// 更新
/// </summary>
void Player::Update(const Input& input, const Camera& camera, Stage& stage)
{
	// 前のフレームと違うstateの場合
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	m_pState->Update(input, camera); 	// stateの更新
	UpdateAngle();	// プレイヤーの向きを更新
	UpdateAnim();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName()); // プレイヤーの情報を描画
#endif
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="moveVec">移動量</param>
void Player::Move(const VECTOR& moveVec)
{
	m_pos = VAdd(m_pos, moveVec);
	m_moveDir = VNorm(moveVec);
	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// プレイヤーの角度を更新
/// </summary>
void Player::UpdateAngle()
{
	m_angle = atan2f(m_moveDir.x, m_moveDir.z);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
