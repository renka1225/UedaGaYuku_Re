#include "DxLib.h"
#include "DebugDraw.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Camera.h"
#include "Stage.h"
#include "EnemyBase.h"
#include "PlayerStateIdle.h"
#include "Player.h"

// 定数
namespace
{
	const std::string kCharaId = "player";							// キャラクターのID名
	const char* kModelFileName = ("data/model/chara/player.mv1");	// モデルのファイル名
	const VECTOR kInitPos = VGet(7425.0, 40.0f, 5190.0f);			// 初期位置
	constexpr float kScale = 0.14f;									// モデルの拡大率
}

Player::Player():
	m_money(0)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData, kCharaId);

	m_pos = kInitPos;
	m_modelHandle = MV1LoadModel(kModelFileName);
}

Player::~Player()
{
	CharacterBase::~CharacterBase();
}

void Player::Init()
{
	CharacterBase::Init();
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	m_pState = std::make_shared<PlayerStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;
	
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();
}

void Player::Update(const Input& input, const Camera& camera, Stage& stage, EnemyBase& enemy)
{
	CharacterBase::Update();

	// 前のフレームと違うstateの場合
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	// 敵との当たり判定をチェックする
	enemy.CheckCharaCol(*this, m_updateCol.bodyStartPos, m_updateCol.bodyEndPos, m_colData.bodyRadius);

	m_pState->Update(input, camera, stage);	// stateの更新
	UpdateAngle();	// 向きを更新
	UpdateAnim();	// アニメーションを更新
	UpdateCol();	// 当たり判定の位置更新
}

void Player::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName()); // プレイヤーの情報を描画
#endif
}

void Player::UpdateAngle()
{
	m_angle = atan2f(m_moveDir.x, m_moveDir.z);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
