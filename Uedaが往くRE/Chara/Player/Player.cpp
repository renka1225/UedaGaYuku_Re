#include "DxLib.h"
#include "DebugDraw.h"
#include "Vec2.h"
#include "Game.h"
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

	constexpr int kMoneyIncrement = 5; // 一度に増える所持金数
}

Player::Player():
	m_money(0),
	m_beforeMoney(0),
	m_addMoney(0)
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

void Player::Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy)
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
	if (pEnemy != nullptr)
	{
		pEnemy->CheckCharaCol(*this, m_updateCol.bodyStartPos, m_updateCol.bodyEndPos, m_colData.bodyRadius);
		/*
		if (m_status)
		{
			pEnemy->CheckHitPunchCol(*this, m_updateCol.armStartPos, m_updateCol.armEndPos, m_colData.armRadius);
			pEnemy->CheckHitKickCol(*this, m_updateCol.legStartPos, m_updateCol.legEndPos, m_colData.legRadius);
		}
		*/
	}

	m_pState->Update(input, camera, stage, pEnemy);	// stateの更新
	UpdateAngle();	// 向きを更新
	UpdateAnim();	// アニメーションを更新
	UpdateCol();	// 当たり判定の位置更新

	UpdateMoney();	// 所持金を更新
}

void Player::Draw()
{
	CharacterBase::Draw();

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName(), m_isGrabWeapon); // プレイヤーの情報を描画
#endif
}

void Player::UpdateAngle()
{
	m_angle = atan2f(m_moveDir.x, m_moveDir.z);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

void Player::UpdateMoney()
{
	m_money += kMoneyIncrement;
	m_money = std::min(m_beforeMoney + m_addMoney, m_money);
}

void Player::AddMoney(int dropMoney)
{
	m_addMoney = dropMoney;
	m_beforeMoney = m_money;
}
