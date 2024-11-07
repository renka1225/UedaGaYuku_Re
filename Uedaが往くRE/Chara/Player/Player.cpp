#include "DxLib.h"
#include "DebugDraw.h"
#include "Vec2.h"
#include "Game.h"
#include "ModelFrameName.h"
#include "Input.h"
#include "LoadCsv.h"
#include "UiBar.h"
#include "Camera.h"
#include "Stage.h"
#include "Weapon.h"
#include "EnemyBase.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "Player.h"

// 定数
namespace
{
	const std::string kCharaId = "player";					// キャラクターのID名
	const VECTOR kInitPos = VGet(7425.0, 40.0f, 5190.0f);	// 初期位置

	constexpr float kScale = 0.14f;			// モデルの拡大率
	constexpr float kDistEnemyGrab = 50.0f;	// 敵を掴める距離

	constexpr int kMaxPossession = 12;	// アイテムの最大所持数
	constexpr int kMoneyIncrement = 5; // 一度に増える所持金数
}

Player::Player(int modelHandle):
	m_money(0),
	m_beforeMoney(0),
	m_addMoney(0),
	m_itemEffectTime(0)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData[CharaType::kPlayer], kCharaId);

	m_modelHandle = modelHandle;
	m_possessItem.resize(kMaxPossession, -1);
	m_pos = kInitPos;
	m_colData[CharaType::kPlayer].bodyUpdateStartPos = m_colData[CharaType::kPlayer].bodyStartPos;
	m_colData[CharaType::kPlayer].bodyUpdateEndPos = m_colData[CharaType::kPlayer].bodyEndPos;
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

void Player::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
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
	if (!pEnemy.empty())
	{
		m_pToEVec.resize(pEnemy.size());

		for (int i = 0; i < pEnemy.size(); i++)
		{
			if (pEnemy[i] == nullptr) continue;

			m_pToEVec[i] = VSub(pEnemy[i]->GetPos(), m_pos);
			pEnemy[i]->CheckCharaCol(*this, m_colData[CharaType::kPlayer], pEnemy[i]->GetEnemyIndex());

			// TODO:範囲内にいる場合、掴みをできるようにする
			float dot = VDot(VNorm(m_pToEVec[i]), VNorm(m_moveDir)); // プレイヤーの方向と位置ベクトルの内積を計算
			bool isGrab = VSize(m_pToEVec[i]) < kDistEnemyGrab && dot > 0.0f;
			if (isGrab)
			{
				m_isPossibleGrabEnemy = true;
			}
			else
			{
				m_isPossibleGrabEnemy = false;
			}
		}
	}

	// 武器との当たり判定をチェックする
	bool isHitWeapon = weapon.CheckWeaopnCol(m_colData[CharaType::kPlayer], *this);
	if (isHitWeapon)
	{
		m_isPossibleGrabWeapon = true;
	}
	else
	{
		m_isPossibleGrabWeapon = false;
	}

	m_itemEffectTime--; // アイテムの効果時間
	if(m_itemEffectTime <= 0)
	{
		DeleteItemEffect(); // アイテムの効果を消す
	}

	m_pState->Update(input, camera, stage, weapon, pEnemy);	// stateの更新
	UpdateAngle();					// 向きを更新
	UpdateAnim();					// アニメーションを更新
	UpdateCol(CharaType::kPlayer);	// 当たり判定の位置更新
	UpdatePosLog();					// 位置ログを更新
	GetFramePos();					// モデルフレーム位置を取得
	UpdateMoney();					// 所持金を更新
}

void Player::Draw()
{
	CharacterBase::Draw();

	// 回避中は残像を表示する
	if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kAvoid)
	{
		DrawAfterImage();
	}

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, m_pState->GetStateName(), m_isNowGrabWeapon); // プレイヤーの情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_colData[CharaType::kPlayer]);	// 全身(紫色)
	//debug.DrawArmCol(m_colData[CharaType::kPlayer]);	// 腕(水色)
	//debug.DrawLegCol(m_colData[CharaType::kPlayer]);	// 脚(黄色)
#endif
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

void Player::AddItem(int itemType)
{
	// アイテムの所持数が最大の場合は、アイテムを取得できないようにする
	auto result = std::find(m_possessItem.begin(), m_possessItem.end(), -1);
	if (result == m_possessItem.end())
	{
		return;
	}

	for (int i = 0; i < m_possessItem.size(); i++)
	{
		// すでにアイテムがある場合は飛ばす
		if (m_possessItem[i] != -1) continue;
		
		// 取得したアイテムを保存する
		m_possessItem[i] = itemType;
		return;
	}
}

void Player::DeleteItem(int selectNum)
{
	m_possessItem[selectNum] = -1;
}

void Player::RecoveryHp(float recoveryRate)
{
	m_hp += m_status.maxHp * recoveryRate;
	m_hp = std::min(m_hp, m_status.maxHp);
}

void Player::RecoveryGauge(float recoveryRate)
{
	m_gauge += m_status.maxGauge * recoveryRate;
	m_gauge = std::min(m_gauge, m_status.maxGauge);
}

void Player::RecoveryHpGauge(float hpRecoveryRate, float gaugeRecoveryRate)
{
	m_hp += m_status.maxHp * hpRecoveryRate;
	m_hp = std::min(m_hp, m_status.maxHp);

	m_gauge += m_status.maxGauge * gaugeRecoveryRate;
	m_gauge = std::min(m_gauge, m_status.maxGauge);
}

void Player::GetFramePos()
{
	m_colData[CharaType::kPlayer].leftShoulderPos = GetModelFramePos(PlayerFrameName::kLeftShoulder.c_str());	// 左肩
	m_colData[CharaType::kPlayer].leftForeArmPos = GetModelFramePos(PlayerFrameName::kLeftForeArm.c_str());		// 左肘
	m_colData[CharaType::kPlayer].leftHandPos = GetModelFramePos(PlayerFrameName::kLeftHandPos.c_str());		// 左手首
	m_colData[CharaType::kPlayer].rightShoulderPos = GetModelFramePos(PlayerFrameName::kRightShoulder.c_str()); // 右肩
	m_colData[CharaType::kPlayer].rightForeArmPos = GetModelFramePos(PlayerFrameName::kRightForeArm.c_str());	// 右肘
	m_colData[CharaType::kPlayer].rightHandPos = GetModelFramePos(PlayerFrameName::kRightHand.c_str());			// 右手首

	m_colData[CharaType::kPlayer].leftUpLegPos = GetModelFramePos(PlayerFrameName::kLeftUpLeg.c_str());			// 左もも
	m_colData[CharaType::kPlayer].leftLegPos = GetModelFramePos(PlayerFrameName::kLeftLeg.c_str());				// 左膝
	m_colData[CharaType::kPlayer].leftFootPos = GetModelFramePos(PlayerFrameName::kLeftFoot.c_str());			// 左足首
	m_colData[CharaType::kPlayer].leftEndPos = GetModelFramePos(PlayerFrameName::kLeftEnd.c_str());				// 左足終点
	m_colData[CharaType::kPlayer].rightUpLegPos = GetModelFramePos(PlayerFrameName::kRightUpLeg.c_str());		// 右もも
	m_colData[CharaType::kPlayer].rightLegPos = GetModelFramePos(PlayerFrameName::kRightLeg.c_str());			// 右膝
	m_colData[CharaType::kPlayer].rightFootPos = GetModelFramePos(PlayerFrameName::kRightFoot.c_str());			// 右足首
	m_colData[CharaType::kPlayer].rightEndPos = GetModelFramePos(PlayerFrameName::kRightEnd.c_str());			// 右足終点
}

void Player::AtkUp(float atkUpRate, int effectTime)
{
	m_itemEffectTime = effectTime;
	m_status.atkPowerPunch1 *= atkUpRate;
}

void Player::DefUp(float defUpRate, int effectTime)
{
	m_itemEffectTime = effectTime;
	//m_status.def *= defUpRate;
}

void Player::DeleteItemEffect()
{
	// TODO:アイテム使用前のステータスに戻す
}
