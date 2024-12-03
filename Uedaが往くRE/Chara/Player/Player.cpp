#include "DxLib.h"
#include "DebugDraw.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "UiBar.h"
#include "Camera.h"
#include "Stage.h"
#include "Weapon.h"
#include "EnemyBase.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "ModelFrameName.h"
#include "Player.h"

// 定数
namespace
{
	const std::string kCharaId = "player";	// キャラクターのID名

	constexpr float kScale = 0.14f;			// モデルの拡大率
	constexpr float kDistEnemyGrab = 50.0f;	// 敵を掴める距離

	constexpr int kMaxPossession = 12;		// アイテムの最大所持数
	constexpr int kMoneyIncrement = 100;	// 一度に増える所持金数

	constexpr float kBattleStartRange = 200.0f;	// バトルが始まる範囲
	constexpr int kBattleStartTime = 60;		// バトルが開始するまでの時間

	constexpr int kInputRetentionFrame = 30;	// 入力の履歴を削除するまでのフレーム数
	constexpr int kInputTimeAdj = 40;			// 入力受付時間調節
}

Player::Player(std::shared_ptr<UiBar> pUi, int modelHandle):
	m_saveStatus(m_status),
	m_money(0),
	m_beforeMoney(100000),
	m_addMoney(0),
	m_itemEffectTime(0),
	m_battleStartCount(kBattleStartTime),
	m_isAddItem(true),
	m_isBattle(false)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData[CharaType::kPlayer], kCharaId);

	m_colData[CharaType::kPlayer].bodyUpdateStartPos = m_colData[CharaType::kPlayer].bodyStartPos;
	m_colData[CharaType::kPlayer].bodyUpdateEndPos = m_colData[CharaType::kPlayer].bodyEndPos;

	m_pUiBar = pUi;
	m_modelHandle = modelHandle;
	m_possessItem.resize(kMaxPossession, -1);

	// セーブデータの情報を適用する
	auto saveData = SaveData::GetInstance().GetSaveData();
	m_pos = saveData.playerPos;
	m_hp = saveData.hp;
	m_gauge = saveData.gauge;
	m_beforeMoney = saveData.money;
	m_enhanceStep = saveData.enhanceStep;
	m_possessItem = saveData.possessItem;
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

	// バトル中でない場合
	if (!m_isBattle)
	{
		// 武器掴み状態を解除する
		m_isNowGrabWeapon = false;
	}

	// 敵がいる場合のみ処理を行う
	if (!pEnemy.empty())
	{
		UpdateEnemyInfo(pEnemy);
	}

	m_currentInputFrame++;

	m_pState->Update(input, camera, stage, weapon, pEnemy);	// stateの更新
	UpdateInputLog(input, m_currentInputFrame);	// 入力情報を更新
	UpdateAngle();					// 向きを更新
	UpdateAnim();					// アニメーションを更新
	UpdateCol(CharaType::kPlayer);	// 当たり判定の位置更新
	UpdateWeaponColInfo(weapon);	// 武器との当たり判定情報を更新する
	UpdatePosLog();					// 位置ログを更新
	GetFramePos();					// モデルフレーム位置を取得
	UpdateItemInfo();				// アイテム情報を更新
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
	debug.DrawPlayerInfo(m_pos, m_hp, m_status, m_pState->GetStateName(), m_isNowGrabWeapon); // プレイヤーの情報を描画
	// 当たり判定描画
	debug.DrawBodyCol(m_colData[CharaType::kPlayer]);	// 全身(紫色)
	//debug.DrawArmCol(m_colData[CharaType::kPlayer]);	// 腕(水色)
	//debug.DrawLegCol(m_colData[CharaType::kPlayer]);	// 脚(黄色)

	// アニメーションのフレーム数表示
	debug.DrawAnimFrame(m_totalAnimTime, m_currenAnimName, m_animData);
#endif
}

void Player::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);
	m_pUiBar->SetPlayerDamage(damage);
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
		m_isAddItem = false;
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

void Player::AtkUp(float atkUpRate, int effectTime)
{
	m_itemEffectTime = effectTime;

	// ステータスを一時保存する
	m_saveStatus = m_status;

	// 攻撃力アップ
	m_status.atkPowerPunch1 *= atkUpRate;
	m_status.atkPowerPunch2 *= atkUpRate;
	m_status.atkPowerPunch3 *= atkUpRate;
	m_status.atkPowerKick *= atkUpRate;
	m_status.atkPowerOneHandWeapon *= atkUpRate;
	m_status.atkPowerTwoHandWeapon *= atkUpRate;
}

void Player::EnhanceHpUp(float upAmount, int money)
{
	m_status.maxHp *= upAmount;
	m_hp *= upAmount;
	m_money -= money;
	m_enhanceStep.nowHpUpStep++;
}

void Player::EnhanceGauge(float upAmount, int money)
{
	m_status.maxGauge *= upAmount;
	m_gauge *= upAmount;
	m_money -= money;
	m_enhanceStep.nowGaugeUpStep++;
}

void Player::EnhanceAtkUp(float upAmount, int money)
{
	m_status.atkPowerPunch1 *= upAmount;
	m_status.atkPowerPunch2 *= upAmount;
	m_status.atkPowerPunch3 *=upAmount;
	m_status.atkPowerKick *= upAmount;
	m_status.atkPowerOneHandWeapon *= upAmount;
	m_status.atkPowerTwoHandWeapon *= upAmount;
	m_money -= money;
	m_enhanceStep.nowAtkUpStep++;
}

void Player::UpdateEnemyInfo(std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	m_pToEVec.resize(pEnemy.size());

	for (int i = 0; i < pEnemy.size(); i++)
	{
		if (pEnemy[i] == nullptr) continue;

		// プレイヤーから敵の位置ベクトルを計算する
		m_pToEVec[i] = VSub(pEnemy[i]->GetPos(), m_pos);

		// 敵との当たり判定をチェックする
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

		UpdateBattle(i); // バトル状態を更新する
	}
}

void Player::UpdateWeaponColInfo(Weapon& weapon)
{
	bool isHitWeapon = weapon.CheckWeaopnCol(m_colData[CharaType::kPlayer], *this);
	// 範囲内に武器がある場合
	if (isHitWeapon)
	{
		// 武器を拾えるようにする
		m_isPossibleGrabWeapon = true;
	}
	else
	{
		m_isPossibleGrabWeapon = false;
	}
}

void Player::UpdateBattle(int enemyIndex)
{
	// 敵に一定距離近づいた場合
	if (!m_isBattle)
	{
		if (VSize(m_pToEVec[enemyIndex]) <= kBattleStartRange)
		{
			m_battleStartCount--;

			// 数秒間敵の範囲内にいた場合、バトル開始状態にする
			if (m_battleStartCount <= 0)
			{
				m_isBattle = true;
			}
		}
		else
		{
			m_battleStartCount = kBattleStartTime;
		}
	}
}

void Player::UpdateItemInfo()
{
	if (m_itemEffectTime > 0)
	{
		m_itemEffectTime--; // アイテムの効果時間を減らす
		m_itemEffectTime = std::max(0, m_itemEffectTime);
	}
	else
	{
		DeleteItemEffect(); // アイテムの効果を消す
	}
}

void Player::DeleteItemEffect()
{
	//m_status = m_saveStatus;
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

void Player::UpdateInputLog(const Input& input, int currentFrame)
{
	if (input.IsTriggered(InputId::kPunch))
	{
		m_inputLog.push_back({ InputId::kPunch, currentFrame});
	}
	if (input.IsTriggered(InputId::kKick))
	{
		m_inputLog.push_back({ InputId::kKick, currentFrame});
	}

#ifdef _DEBUG
	// 入力コマンドをデバックで確認
	//for (int i = 0; i < m_inputLog.size(); i++)
	//{
	//	printfDx("%d.入力コマンド:%s\n", i, m_inputLog[i].button.c_str());
	//}
#endif

	// 履歴を削除
	for (auto it = m_inputLog.begin(); it != m_inputLog.end();)
	{
		// 入力受付時間
		const int inputReceptionTime = m_animData[m_currenAnimName].startupFrame + m_animData[m_currenAnimName].activeFrame + kInputTimeAdj;

		// 入力をチェック 
		if (currentFrame - it->frameCount > inputReceptionTime)
		{
			// 削除する
			it = m_inputLog.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool Player::CheckCommand(const std::vector<std::string>& command, const std::vector<CommandInput>& inputLog)
{
	// ログがない場合はチェックしない
	if (inputLog.empty()) return false;

	// コマンドの最終入力を確認する
	int index = command.size() - 1;
	// 最新の入力時間
	int currentTime = inputLog.back().frameCount;

	// MEMO;rbegin()は逆イテレータを返す
	for (auto it = inputLog.rbegin(); it != inputLog.rend(); it++)
	{
		if (it->button == command[index])
		{
			// 入力が確認できた場合
			index--;
			// すべてのコマンドが一致している場合
			if (index < 0) return true;
		}
	}
	return false;
}
