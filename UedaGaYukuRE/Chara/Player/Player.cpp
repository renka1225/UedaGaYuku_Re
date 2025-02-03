#include "DxLib.h"
#include "DebugDraw.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "UiBar.h"
#include "UiMain.h"
#include "Camera.h"
#include "Stage.h"
#include "Weapon.h"
#include "EnemyBase.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "ModelFrameName.h"
#include "Player.h"
#include <random>

// 定数
namespace
{
	const std::string kCharaId = "player";	// キャラクターのID名

	constexpr float kScale = 0.14f;				// モデルの拡大率
	constexpr float kRangeFoundEnemy = 50.0f;	// 敵を認識する範囲
	constexpr float kRangeSpecial = 30.0f;		// 必殺技を出せる範囲
	constexpr float kDistWeaponGrab = 20.0f;	// 武器を掴める距離
	constexpr float kMoveAttack = 0.3f;			// 攻撃時の移動量

	constexpr int kMaxPossession = 12;			// アイテムの最大所持数
	constexpr int kMoneyIncrement = 50;			// 一度に増える所持金数

	constexpr float kMaxRecoveryRate = 10.0f;	// 最大の回復割合
	constexpr int kDecreaseMinSpecialGauge = 2;	// ダメージを受けた際に減るゲージの最小量
	constexpr int kDecreaseMaxSpecialGauge = 8;	// ダメージを受けた際に減るゲージの最大量

	constexpr float kBattleStartRange = 150.0f;	// バトルが始まる範囲
	constexpr int kBattleStartTime = 50;		// バトルが開始するまでの時間

	/*攻撃*/
	constexpr int kInputRetentionFrame = 20;	// 入力の履歴を削除するまでのフレーム数
	constexpr int kInputTimeAdj = 30;			// 入力受付時間調節
	constexpr float kChangeAngleDot = -0.5f;	// プレイヤーの角度を調節する範囲
	constexpr float kBattleStartChangeAngleDot = 1.0f; // バトル開始時のプレイヤーの角度を調整する範囲

	/*チュートリアル*/
	constexpr int kTutoMinNum = 1;		// チュートリアルの最小回数
	constexpr int kTutoMidiumNum = 3;	// チュートリアルの中回数
	constexpr int kTutoMaxNum = 5;		// チュートリアルの最大回数
	constexpr int kTutoChangeTime = 30;	// チュートリアルの切り替え時間
	constexpr int kKnowledgeNum = 4;	// 心得の最大表示数
	constexpr int kTutoTalkNum = 2;		// チュートリアル時の会話数

	/*特殊バトル*/
	const VECTOR kSpecialBattleInitPos = VGet(8876.0f, 40.0f, 2767.0f);	 // バトル開始位置
}

Player::Player(std::shared_ptr<UiBar> pUi, std::shared_ptr<UiMain> pUiMain, int modelHandle):
	m_saveStatus(m_status),
	m_deadEnemyNum(0),
	m_money(0),
	m_beforeMoney(0),
	m_addMoney(0),
	m_currentInputFrame(0),
	m_itemEffectTime(0),
	m_isAddItem(true),
	m_isSpecial(false),
	m_isTalk(true),
	m_isNowTalk(false),
	m_battleStartCount(kBattleStartTime),
	m_isBattle(false)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kCharaId);
	LoadCsv::GetInstance().LoadColData(m_colData[CharaType::kPlayer], kCharaId);

	m_colData[CharaType::kPlayer].bodyUpdateStartPos = m_colData[CharaType::kPlayer].bodyStartPos;
	m_colData[CharaType::kPlayer].bodyUpdateEndPos = m_colData[CharaType::kPlayer].bodyEndPos;

	// セーブデータの情報を適用する
	ApplySaveData();

	m_pUiBar = pUi;
	m_pUiMain = pUiMain;
	m_modelHandle = modelHandle;
	m_saveStatus = m_status;
	m_possessItem.resize(kMaxPossession, -1);
}

Player::~Player()
{
	CharacterBase::~CharacterBase();
}

void Player::Init()
{
	CharacterBase::Init();
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	// 状態を適用する
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
		m_isNowGrabWeapon = false;
		m_isOnDamage = false;
	}
	// バトル中の場合
	else
	{
		m_isTalk = false;
		m_isNowTalk = false;
	}

	// 会話中の場合
	if (m_isNowTalk || m_tutorial.isNowKnowledge || m_tutorial.isTalk)
	{
		m_isPossibleMove = false;
	}

	// 敵がいる場合のみ処理を行う
	if (!pEnemy.empty())
	{
		UpdateEnemyInfo(pEnemy);
	}

	m_currentInputFrame++;

	m_pState->Update(input, camera, stage, weapon, pEnemy);	// stateの更新
	UpdateInputLog(input, m_currentInputFrame);				// 入力情報を更新
	UpdateAngle();					// 向きを更新
	UpdateAnim();					// アニメーションを更新
	UpdateCol(CharaType::kPlayer);	// 当たり判定の位置更新
	UpdateWeaponColInfo(weapon);	// 武器との当たり判定情報を更新する
	UpdatePosLog();					// 位置ログを更新
	GetFramePos();					// モデルフレーム位置を取得
	UpdateItemInfo();				// アイテム情報を更新
	UpdateMoney();					// 所持金を更新

//#ifdef _DEBUG // デバッグ
	if (input.IsPressing(InputId::kDebugAddMoney) && input.IsTriggered(InputId::kA))
	{
		AddDecreaseMoney(1000); // 所持金を追加
	}
//#endif
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

	// 減らすゲージ量
	int decreaseGauge = GetRand((kDecreaseMaxSpecialGauge - kDecreaseMinSpecialGauge)) + kDecreaseMinSpecialGauge;
	UpdateGauge(static_cast<float>(-decreaseGauge));
}

void Player::AdjPosAttack()
{
	int nearEnemyIndex = -1;
	nearEnemyIndex = FindNearEnemy(kRangeFoundEnemy);

	// 敵が範囲内にいる場合、敵に近づく
	bool isNear = nearEnemyIndex != -1 && VSize(m_pToEVec[nearEnemyIndex]) < kRangeFoundEnemy;
	if (isNear)
	{
		VECTOR scale = VScale(m_pToEVec[nearEnemyIndex], kMoveAttack);
		scale.y = 0.0f;
		m_pos = VAdd(m_pos, scale);
	}
}

void Player::UpdateAngleNearEnemy()
{
	// 1番近い敵を探す
	int nearEnemyIndex = -1;
	nearEnemyIndex = FindNearEnemy(kBattleStartChangeAngleDot);

	// 範囲内に敵が存在する場合、1番近い敵の方を向く
	if (nearEnemyIndex != -1)
	{
		// 敵への方向ベクトルを正規化
		VECTOR dirToEnemy = VNorm(m_pToEVec[nearEnemyIndex]);
		m_moveDir = dirToEnemy;
	}

	CharacterBase::UpdateAngle();

	return;
}

void Player::UpdateBattleEnd()
{
	m_isPossibleMove = false;
	m_pState->UpdateBattleEnd();

	// アニメーションをスローモーションで再生
	SlowAnim();
}

int Player::FindNearEnemy(float range)
{
	for (int i = 0; i < m_pToEVec.size(); i++)
	{
		float dot = VDot(VNorm(m_pToEVec[i]), VNorm(m_moveDir)); // プレイヤーの方向と位置ベクトルの内積を計算
		if (dot > kChangeAngleDot)
		{
			return i;
		}
	}

	return -1;
}

void Player::UpdateMoney()
{
	if (m_addMoney <= 0) return;
	m_money += kMoneyIncrement;
	m_money = std::min(m_money, m_beforeMoney + m_addMoney);
}

void Player::AddDecreaseMoney(int dropMoney)
{
	m_pUiMain->SetAnimMoneyUi();

	Sound::GetInstance().PlaySe(SoundName::kSe_getMoney); // SE再生

	m_beforeMoney = m_money;
	m_addMoney = dropMoney;
}

void Player::AddItem(int itemType)
{
	// アイテムの所持数が最大の場合は、アイテムを取得できないようにする
	auto result = std::find(m_possessItem.begin(), m_possessItem.end(), -1);
	if (result == m_possessItem.end())
	{
		if (!m_isNowTalk)
		{
			m_pUiMain->SetMaxItemUi();
		}
		m_isAddItem = false;
		return;
	}

	Sound::GetInstance().PlayBackSe(SoundName::kSe_getItem);

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

void Player::UpdateGauge(float changeAmount)
{
	m_gauge += changeAmount;
	m_gauge = std::min(m_gauge, m_status.maxGauge);
	m_gauge = std::max(0.0f, m_gauge);
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

void Player::UpdateTutorial(const Input& input, EnemyBase& pEnemy)
{
	// チュートリアル終了していたら飛ばす
	if (m_tutorial.isEndTutorial) return;

#ifdef _DEBUG // デバッグコマンド
	if (input.IsTriggered(InputId::kDebugTutorial))
	{
		m_tutorial.currentNum++;
		if (m_tutorial.currentNum > TutorialNum::kTutoNum)
		{
			m_tutorial.isEndTutorial = true;
			m_isBattle = false;
		}
	}
#endif // _DEBUG

	// HPを回復し続ける
	m_hp = std::min(m_hp, m_status.maxHp);
	RecoveryHp(kMaxRecoveryRate);

	m_tutorial.tutorialChangeTime--;

	switch (m_tutorial.currentNum)
	{
	case TutorialNum::kTuto_0:
		UpdateTuto0(input);
		break;
	case TutorialNum::kTuto_1:
		UpdateTuto1(input);
		break;
	case TutorialNum::kTuto_2:
		UpdateTuto2(input);
		break;
	case TutorialNum::kTuto_3:
		UpdateTuto3(input);
		break;
	case TutorialNum::kTuto_4:
		UpdateTuto4(input, pEnemy);
		break;
	case TutorialNum::kTuto_5:
		UpdateTuto5(input);
		break;
	}

	m_isTalk = false;
}

void Player::ChangeTutorial(const Input& input)
{
	// 切り替えの演出を行う
	if (!m_tutorial.isNowChange)
	{
		m_tutorial.isNowChange = true;
		m_tutorial.tutorialChangeTime = kTutoChangeTime;
		Sound::GetInstance().PlaySe(SoundName::kSe_tuto_change);
	}

	// 心得表示
	if (m_tutorial.isNowKnowledge)
	{
		return;
	}

	// 次のチュートリアルに切り替える
	if (m_tutorial.tutorialChangeTime < 0)
	{
		m_tutorial.isNowChange = false;
		m_tutorial.currentNum++;

		if (m_tutorial.currentNum == TutorialNum::kTuto_3)
		{
			m_tutorial.isNowKnowledge = true;
		}
	}
}

void Player::SetSpecialBattleInit()
{
	m_pos = kSpecialBattleInitPos;
	m_moveDir = m_pToEVec[0];
	MV1SetRotationXYZ(m_modelHandle, m_moveDir);
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

		UpdateBattle(i); // バトル状態を更新する

		// 敵が範囲内にいる場合
		if (VSize(m_pToEVec[i]) <= kRangeSpecial)
		{
			// チュートリアル前は必殺技を出せないようにする
			if (!m_tutorial.isEndTutorial && m_tutorial.currentNum < TutorialNum::kTuto_4) return;

			// ゲージが溜まっている場合
			if (m_gauge >= GetStatus().maxGauge)
			{
				m_isSpecial = true; // 必殺技を出せるようにする
				continue;
			}
		}
		
		m_isSpecial = false;
	}
}

void Player::UpdateWeaponColInfo(Weapon& weapon)
{
	// チュートリアルの途中までは掴めないようにする
	if (!m_tutorial.isEndTutorial && m_tutorial.currentNum <= TutorialNum::kTuto_2) return;

	bool isHitWeapon = weapon.CheckWeaponCol(m_colData[CharaType::kPlayer], *this);

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

bool Player::IsNearWeapon(const VECTOR& weaponPos)
{
	// 必殺技中は飛ばす
	if (GetCurrentAnim() == AnimName::kSpecialAtk1 || GetCurrentAnim() == AnimName::kSpecialAtk2) return false;

	const float kNearDist = kDistWeaponGrab;
	float dist = VSize(VSub(weaponPos, m_pos));

	if(dist <= kNearDist) return true;

	return false;
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

	// アイテムを取得できる場合
	auto result = std::find(m_possessItem.begin(), m_possessItem.end(), -1);
	if (result != m_possessItem.end())
	{
		m_isAddItem = true;
		return;
	}
}

void Player::DeleteItemEffect()
{
	m_status = m_saveStatus;
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
	int index = static_cast<int>(command.size()) - 1;
	// 最新の入力時間
	int currentTime = inputLog.back().frameCount;

	// MEMO:rbegin()は逆イテレータを返す
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

void Player::UpdateAngle()
{
	// 攻撃中の場合
	if (m_isAttack && m_isBattle)
	{
		// 1番近い敵を探す
		int nearEnemyIndex = -1;
		nearEnemyIndex = FindNearEnemy(kChangeAngleDot);

		// 範囲内に敵が存在する場合、1番近い敵の方を向く
		if (nearEnemyIndex != -1 && VSize(m_pToEVec[nearEnemyIndex]) < kRangeFoundEnemy)
		{
			// 敵への方向ベクトルを正規化
			VECTOR dirToEnemy = VNorm(m_pToEVec[nearEnemyIndex]);
			m_moveDir = dirToEnemy;
		}

		CharacterBase::UpdateAngle();
	}
	else
	{
		// 通常の処理
		CharacterBase::UpdateAngle();
	}
}

void Player::ApplySaveData()
{
	auto saveData = SaveData::GetInstance().GetSaveData();
	m_pos = saveData.playerPos;
	m_hp = saveData.hp;
	m_gauge = saveData.gauge;
	m_money = saveData.money;
	m_enhanceStep = saveData.enhanceStep;
	m_possessItem = saveData.possessItem;
	m_deadEnemyNum = saveData.deadEnemyNum;
	m_tutorial.isEndTutorial = saveData.isEndTutorial;

	if (m_tutorial.isEndTutorial)
	{
		m_tutorial.isNowKnowledge = false;
	}
}

void Player::UpdateTuto0(const Input& input)
{
	// 会話中
	if (m_tutorial.isTalk)
	{
		if (input.IsTriggered(InputId::kA))
		{
			// 次の会話に移行
			m_tutorial.talkNum++;

			// 会話がすべて終わったらチュートリアルへ
			if (m_tutorial.talkNum >= kTutoTalkNum)
			{
				m_tutorial.isTalk = false;
				m_tutorial.isNowKnowledge = true;
				m_tutorial.currentNum++;
			}
		}
		return;
	}

	// Aボタンを押したら、会話開始
	if (input.IsTriggered(InputId::kTuto))
	{
		// 会話状態に移行
		m_tutorial.isTalk = true;
		// 心得を非表示にする
		m_tutorial.isNowKnowledge = false;
		m_tutorial.currentKnowledge++;

		return;
	}

	// 心得を表示
	m_tutorial.isNowKnowledge = true;
}

void Player::UpdateTuto1(const Input& input)
{
	// 心得表示
	if (m_tutorial.isNowKnowledge)
	{
		if (input.IsTriggered(InputId::kTuto))
		{
			m_tutorial.currentKnowledge++;
			m_tutorial.isNowKnowledge = false;
		}
		return;
	}

	// 移動
	if (GetCurrentAnim() == AnimName::kWalk)
	{
		m_tutorial.isMove = true;
	}
	// ダッシュ
	if (GetCurrentAnim() == AnimName::kRun)
	{
		m_tutorial.isDush = true;
	}
	// カメラ移動
	DINPUT_JOYSTATE cameraInput;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &cameraInput);
	if (cameraInput.Rx != 0 || cameraInput.Ry != 0)
	{
		m_tutorial.isCameraMove = true;
	}

	// 次のチュートリアルに移る
	if (m_tutorial.isMove && m_tutorial.isDush && m_tutorial.isCameraMove)
	{
		ChangeTutorial(input);
	}
}

void Player::UpdateTuto2(const Input& input)
{
	// パンチ攻撃
	if (input.IsTriggered(InputId::kPunch))
	{
		if (m_isAttack) return;
		m_tutorial.currentPunch++;
		m_tutorial.currentPunch = std::min(kTutoMaxNum, m_tutorial.currentPunch);

		if (m_tutorial.currentPunch < kTutoMaxNum) return;
		m_tutorial.isPunch = true;
	}
	// キック攻撃
	if (input.IsTriggered(InputId::kKick))
	{
		if (m_isAttack) return;
		m_tutorial.currentKick++;
		m_tutorial.currentKick = std::min(kTutoMidiumNum, m_tutorial.currentKick);

		if (m_tutorial.currentKick < kTutoMidiumNum) return;
		m_tutorial.isKick = true;
	}
	// 回避
	if (input.IsTriggered(InputId::kAvoid))
	{
		m_tutorial.currentAvoid++;
		m_tutorial.currentAvoid = std::min(kTutoMidiumNum, m_tutorial.currentAvoid);

		if (m_tutorial.currentAvoid < kTutoMidiumNum) return;
		m_tutorial.isAvoid = true;
	}
	// ガード
	if (input.IsReleased(InputId::kGuard))
	{
		m_tutorial.currentGuard++;
		m_tutorial.currentGuard = std::min(kTutoMidiumNum, m_tutorial.currentGuard);

		if (m_tutorial.currentGuard < kTutoMidiumNum) return;
		m_tutorial.isGuard = true;
	}

	// 次のチュートリアルに移る
	if (m_tutorial.isPunch && m_tutorial.isKick && m_tutorial.isAvoid && m_tutorial.isGuard)
	{
		ChangeTutorial(input);
	}
}

void Player::UpdateTuto3(const Input& input)
{
	// 心得表示
	if (m_tutorial.isNowKnowledge)
	{
		if (input.IsTriggered(InputId::kTuto))
		{
			m_tutorial.currentKnowledge++;
			m_tutorial.isNowKnowledge = false;
		}
		return;
	}

	// 武器掴み
	if (m_isNowGrabWeapon)
	{
		m_tutorial.currentGrab++;
		m_tutorial.currentGrab = std::min(m_tutorial.currentGrab, kTutoMinNum);

		if (m_tutorial.currentPunch < kTutoMinNum) return;
		m_tutorial.isGrab = true;
	}
	// 武器攻撃
	if (input.IsTriggered(InputId::kPunch) && m_isNowGrabWeapon)
	{
		if (m_isAttack) return;
		m_tutorial.currentWeaponAtk++;
		m_tutorial.currentWeaponAtk = std::min(m_tutorial.currentWeaponAtk, kTutoMidiumNum);

		if (m_tutorial.currentWeaponAtk < kTutoMidiumNum) return;
		m_tutorial.isWeaponAtk = true;
	}

	// 次のチュートリアルに移る
	if (m_tutorial.currentGrab && m_tutorial.currentWeaponAtk)
	{
		ChangeTutorial(input);
	}
}

void Player::UpdateTuto4(const Input& input, EnemyBase& pEnemy)
{
	// ヒートゲージを最大にする
	RecoveryGauge(kMaxRecoveryRate);

	if (input.IsTriggered(InputId::kSpecial) && m_isSpecial)
	{
		m_tutorial.currentHeat++;
		m_tutorial.currentHeat = std::min(m_tutorial.currentHeat, kTutoMinNum);

		if (m_tutorial.currentHeat < kTutoMinNum) return;
		m_tutorial.isHeat = true;
	}

	// 必殺技アニメーションが終了したら、次のチュートリアルに移る
	bool isEndAnim = m_currenAnimName != AnimName::kSpecialAtk1 && m_currenAnimName != AnimName::kSpecialAtk2;
	if (m_tutorial.isHeat && !m_isAttack && isEndAnim)
	{
		pEnemy.RecoveryMaxHp();
		ChangeTutorial(input);
	}
}

void Player::UpdateTuto5(const Input& input)
{
	// バトル終了
	if (!m_isBattle && !m_tutorial.isNowKnowledge && !m_tutorial.isEndTutorial)
	{
		m_tutorial.isNowKnowledge = true;
	}

	// 心得表示
	if (m_tutorial.isNowKnowledge)
	{
		// Aボタンを押したら次を表示
		if (input.IsTriggered(InputId::kTuto))
		{
			m_tutorial.currentKnowledge++;

			// すべて表示し終わった場合
			if (m_tutorial.currentKnowledge > kKnowledgeNum)
			{
				// チュートリアルを終了する
				m_tutorial.isNowKnowledge = false;
				m_tutorial.isEndTutorial = true;
			}
		}
		return;
	}

	// TODO:会話パートに入る
}