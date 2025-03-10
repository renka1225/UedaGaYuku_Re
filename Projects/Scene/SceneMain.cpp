﻿#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Sound.h"
#include "EffectManager.h"
#include "UiMain.h"
#include "UiBar.h"
#include "UiConversation.h"
#include "Camera.h"
#include "CharacterBase.h"
#include "Player.h"
#include "Npc.h"
#include "EnemyBase.h"
#include "EnemySpecial.h"
#include "EnemyAI.h"
#include "ObjectBase.h"
#include "Weapon.h"
#include "Item.h"
#include "Stage.h"
#include "EventData.h"
#include "SceneMenu.h"
#include "SceneTitle.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneMain.h"
#include <unordered_set>
#include <random>
#include <windows.h>

// 定数
namespace
{
	/*モデルハンドル*/
	const std::string kHandlePath = "data/model/chara/";			  // モデルハンドルパス
	const std::string kPlayerHandlePath = kHandlePath + "player.mv1"; // プレイヤーのモデルハンドルパス
	const std::string kEnemyHandlePath = kHandlePath + "enemy_";	  // 敵のモデルハンドルパス

	/*敵名*/
	const std::map<int, std::string> kEnemyName
	{
		{CharacterBase::CharaType::kEnemy_boss, "Ohara"},
		{CharacterBase::CharaType::kEnemy_tuto, "Tanaka"},
		{CharacterBase::CharaType::kEnemy_bob, "Bob"},
		{CharacterBase::CharaType::kEnemy_sato, "Sato"},
		{CharacterBase::CharaType::kEnemy_abe, "Abe"}
	};

	/*特殊敵のID*/
	const std::map<int, std::string> kSpecialEnemyId
	{
		{CharacterBase::CharaType::kEnemy_bob, "enemy_bob"},
		{CharacterBase::CharaType::kEnemy_sato, "enemy_sato"},
		{CharacterBase::CharaType::kEnemy_abe, "enemy_abe"}
	};

	constexpr int kModelNum = 10;				// 読み込むモデルの数
	constexpr int kMobEnemyNum = 3;				// 読み込むモブ敵の数
	constexpr int kCreateTutoEnemyTalkNum = 2;	// チュートリアル敵を生成するタイミング
	constexpr int kSpecialEnemyAddIndex = 6;	// 特殊敵の番号を調整

	constexpr int kEnemyMaxNum = 3;		// 1度に出現する最大の敵数
	constexpr int kEnemyKindNum = 3;	// 敵の種類
	constexpr int kEnemySpecialNum = 3;	// 特殊敵の数
	constexpr int kEnemyNamekind = 31;	// 敵名の種類
	constexpr int kClearEnemyNum = 1;	// クリア条件

	constexpr float kRecoveryMaxRate = 100.0f;	// 回復の最大割合
	constexpr int kRecoveryMoney = 1000;		// 回復に必要な金額
	constexpr int kGetItemMoney = 2000;			// アイテム購入に必要な金額

	constexpr int kLoadingTime = 400;				// ロード時間
	constexpr int kLoadingBeforeBattleTime = 120;	// バトル前のロード時間
	constexpr int kFirstSpawnTime = 120;			// ゲーム開始からチュートリアルが始まるまでの時間
	constexpr int kEnemySpawnMinTime = 300;			// 敵がスポーンするまでの最小時間
	constexpr int kEnemySpawnMaxTime = 1000;		// 敵がスポーンするまでの最大時間
	constexpr float kEnemyExtinctionDist = 2500.0f;	// 敵が消滅する範囲

	constexpr int kBattleStartStagingTime = 120;		// バトル開始時の演出時間
	constexpr int kSpecialBattleStartStagingTime = 250;	// 特殊バトル開始時の演出時間
	constexpr int kBattleEndStagingTime = 180;			// バトル終了時の演出時間
	constexpr int kEndingTime = 150;					// エンディングの時間
	constexpr int kTalkDispTime = 3;					// 会話を表示させる最低限の時間

	const std::string kTutoTalkId = "TUTO_";					// チュートリアル会話のID
	const std::string kCursorId = "cursor_main_talkSelect";		// 会話の選択肢カーソルのID
	const std::string kChoiceCursorId = "cursor_main_choice";	// 2択のカーソルのID
	constexpr float kTalkSelectCursorInterval = 98.0f;			// 会話の選択肢カーソルの表示間隔
	constexpr float kChoiceCursorInterval = 164.0f;				// 2択カーソルの表示間隔

	/*影*/
	constexpr int kShadowMapSize = 4096;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = VGet(2000.0f, 0.0f, 2000.0f);		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = VGet(10000.0f, 30.0f, 12000.0f);	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = VGet(0.5f, -2.0f, -2.0f);					// ライト方向
}

SceneMain::SceneMain() :
	m_nowTalkId(""),
	m_talkSelect(TalkSelect::kBattle),
	m_currentEnemyNum(0),
	m_enemySpawnTime(0),
	m_battleStartStagingTime(0),
	m_battleEndStagingTime(0),
	m_endingTime(0),
	m_loadingTime(0),
	m_mainSceneTime(0),
	m_isTalking(false),
	m_isDispTalkSelect(false),
	m_isDispBattleStart(false),
	m_isBattleEndStaging(false),
	m_isEnding(false),
	m_isPause(false),
	m_isLoading(true),
	m_isTutorial(false),
	m_isCreateTutoEnemy(false),
	m_isLastBattle(false)
{
	SetUseASyncLoadFlag(true); 	// 非同期読み込み設定に変更

	m_modelHandle.resize(CharacterBase::CharaType::kCharaNum);
	m_pEnemy.resize(kEnemyKindNum);
	m_pEnemySpecial.resize(kEnemySpecialNum);

	m_pUiMain = std::make_shared<UiMain>();
	m_choiceSelect = Choice::kYes;

	LoadModelHandle();	// モデルを読み込む
}

SceneMain::~SceneMain()
{
	// サウンド停止
	Sound::GetInstance().StopBgm(SoundName::kBgm_congestion);
	Sound::GetInstance().StopBgm(SoundName::kBgm_battle);
	Sound::GetInstance().StopBgm(SoundName::kBgm_bossBattle);
	Sound::GetInstance().StopBgm(SoundName::kBgm_battleEnd);

	DeleteShadowMap(m_shadowMap); // シャドウマップの削除

	m_pEnemy.clear();
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
	for (auto& handle : m_modelHandle)
	{
		MV1DeleteModel(handle);
	}

	// 全エフェクトを停止する
	EffectManager::GetInstance().AllStop();
}

void SceneMain::Init()
{
	// ロード中、ポーズ中は初期化を行わない
	if (m_isLoading || m_isPause) return;

	m_pPlayer->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
	m_pUiBar->Init();
	m_pItem->Init();
	m_isPause = false;
}

std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	// ロード中
	if (m_isLoading) return UpdateLoading(input);
	// エンディング中
	if (m_isEnding && !m_isBattleEndStaging) return UpdateEndingStaging();
	// メニュー表示
	if (input.IsTriggered(InputId::kMenu)) return UpdateMenu(input);

	m_mainSceneTime++;

	// チュートリアル中
	if (m_isTutorial) UpdateTutorial(input);
	// ゲームオーバー
	if (m_pPlayer->GetIsDead()) UpdateGameover();

	// バトル中は会話できないようにする
	if (m_pPlayer->GetIsBattle()) m_pPlayer->SetIsTalk(false);

	UpdateBattle();				// バトル更新
	CheckEventTrigger(input);	// イベントトリガーのチェック
	UpdateTalk(input);			// 会話状態を更新

	m_pStage->Update();
	m_pPlayer->Update(input, *m_pCamera, *m_pStage, *m_pWeapon, m_pEnemy);
	m_pNpc->Update(*m_pStage, *m_pPlayer);
	m_pItem->Update(*m_pPlayer);
	m_pWeapon->Update(*m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);
	m_pUiBar->Update();

	for (auto& enemy : m_pEnemySpecial)
	{
		if (enemy == nullptr) continue;
		enemy->Update(*m_pStage, *m_pPlayer);
	}

	EffectManager::GetInstance().Update(); 	// エフェクトの更新
	UpdateSound();	// サウンド更新

#ifdef _DEBUG
	DebugCommand(input); // デバックコマンド
#endif

	return shared_from_this();
}

void SceneMain::Draw()
{
	// ロード中
	if (m_isLoading)
	{
		m_pUiMain->DrawLoading();
		return;
	}

	// エンディング演出を表示
	if (m_isEnding && !m_isBattleEndStaging)
	{
		m_pUiMain->DrawEnding();
		return;
	}

	// ステージモデル表示
	m_pStage->DrawSkyDoom();
	DrawSetUpShadow();

	// 敵UI表示
	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->DrawUi();
	}

	// エフェクト表示
	EffectManager::GetInstance().Draw();

	// バトル開始の演出を表示
	if (m_battleStartStagingTime > 0)
	{
		m_pUiMain->DrawBattleStart(m_pEnemy[0]->GetEnemyIndex());
	}
	// バトル終了の演出を表示
	if (m_battleEndStagingTime > 0)
	{
		m_pUiMain->DrawBattleEnd(*m_pPlayer, m_battleEndStagingTime);
	}

	// バトル中UI表示
	m_pUiMain->DrawBattleUi(*m_pPlayer);
	// アイテム最大所持UI表示
	m_pUiMain->DrawMaxItem();

	// チュートリアルが終わったかどうか
	bool isTuto = !m_pPlayer->GetTutoInfo().isEndTutorial && m_pPlayer->GetTutoInfo().currentNum <= Player::TutorialNum::kTuto_2;
	// 必殺技中かどうか
	bool isSpecial = m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk1 || m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk2;
	// 特定の状態中のみ表示する
	if (!m_isBattleEndStaging || !isTuto && isSpecial)
	{
		m_pWeapon->DrawWeaponUi(); // 武器UI表示
	}

	m_pUiBar->DrawPlayerSpecial(*m_pPlayer); // 必殺技表示
	m_pUiBar->DrawPlayerHpBar(*m_pPlayer, m_pPlayer->GetStatus().maxHp);		// プレイヤーHP表示
	m_pUiBar->DrawPlayerGaugeBar(*m_pPlayer, m_pPlayer->GetStatus().maxGauge);	// プレイヤーゲージ表示
	
	if (!m_isLastBattle)
	{
		m_pUiMain->DrawMoneyUi(m_pPlayer->GetMoney()); // 所持金UI表示
	}

	DrawTalk();		// 会話表示
	DrawExplain();	// 説明表示

	// 特定の状態の場合はミニマップを表示しない
	bool isNotDrawMap = !m_isBattleEndStaging || !m_isLastBattle || m_isEnding || m_isTutorial || m_pPlayer->GetIsNowTalk();
	if (!isNotDrawMap)
	{
		// ミニマップを表示
		//m_pUi->DrawMiniMap(*m_pPlayer, m_pEnemy);
	}

	// フェード
	DrawFade();
	
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
	m_pEventData->Draw();
	DrawFormatString(0, 550, Color::kColorW, "倒した敵数:%d\n", m_pPlayer->GetDeadEnemyNum());
	//TestDrawShadowMap(m_shadowMap[Shadow::kStage], 0, 0, 480, 270);
#endif
}

void SceneMain::LoadModelHandle()
{
	// プレイヤー
	m_modelHandle[CharacterBase::CharaType::kPlayer] = MV1LoadModel(kPlayerHandlePath.c_str());

	// 敵
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 2桁にそろえる
		char enemyId[kMobEnemyNum];
		sprintf_s(enemyId, "%02d", (i + 1));
		m_modelHandle[(i + 1)] = MV1LoadModel((kEnemyHandlePath + std::string(enemyId) + ".mv1").c_str());
	}

	// ラスボス
	m_modelHandle[CharacterBase::CharaType::kEnemy_boss] = MV1LoadModel((kEnemyHandlePath + "boss.mv1").c_str());
	// チュートリアル敵
	m_modelHandle[CharacterBase::CharaType::kEnemy_tuto] = MV1LoadModel((kEnemyHandlePath + "tuto.mv1").c_str());
	// Bob
	m_modelHandle[CharacterBase::CharaType::kEnemy_bob] = MV1LoadModel((kEnemyHandlePath + "bob.mv1").c_str());
	// Sato
	m_modelHandle[CharacterBase::CharaType::kEnemy_sato] = MV1LoadModel((kEnemyHandlePath + "sato.mv1").c_str());
	// Abe
	m_modelHandle[CharacterBase::CharaType::kEnemy_abe] = MV1LoadModel((kEnemyHandlePath + "abe.mv1").c_str());
	// NPC
	m_modelHandle[CharacterBase::CharaType::kNpc] = MV1LoadModel((kHandlePath + "npc.mv1").c_str());
}

#ifdef _DEBUG
std::shared_ptr<SceneBase> SceneMain::DebugCommand(const Input& input)
{
	// クリア画面に遷移
	if (input.IsTriggered(InputId::kDebugClear))
	{
		return std::make_shared<SceneClear>();
	}
	// エンディングに遷移
	else if (input.IsTriggered(InputId::kDebugEnding))
	{
		m_isEnding = true;
	}
	// ゲームオーバー画面に遷移
	else if (input.IsTriggered(InputId::kDebugGameover))
	{
		return std::make_shared<SceneGameover>();
	}

	return shared_from_this();
}
#endif

void SceneMain::Loading()
{
	// 非同期読み込み数を確認
	int aSyncLoadNum = GetASyncLoadNum();
	m_loadingTime++;

	// すべての読み込みが終了した場合
	if (aSyncLoadNum == 0)
	{
		// 一定時間経過したら読み込み演出を終了する
		if (m_loadingTime < kLoadingTime) return;

		// 同期読み込み設定に変更
		SetUseASyncLoadFlag(false);
		m_isLoading = false;
		m_loadingTime = 0;

		// ロード完了後の処理を行う
		InitAfterLoading();
	}
}

void SceneMain::LoadingBeforeBattle()
{
	m_loadingTime++;
	if (m_loadingTime < kLoadingBeforeBattleTime) return;

	m_isLoading = false;
	m_loadingTime = 0;

	// プレイヤー位置変更
	m_pPlayer->SetSpecialBattleInit();
	// カメラ位置変更
	m_pCamera->SetSpecialBattleInit();

	// ロードが終わったら敵を生成する
	CreateBossEnemy();
	m_pPlayer->SetIsBattle(true);
}

std::shared_ptr<SceneBase> SceneMain::UpdateLoading(const Input& input)
{
	if (m_isLastBattle)
	{
		LoadingBeforeBattle();
	}
	else
	{
		Loading();
	}

	m_pUiMain->UpdateLoading(input);

	return shared_from_this();
}

void SceneMain::InitAfterLoading()
{
	m_pUiBar = std::make_shared<UiBar>();
	m_pUiConversation = std::make_shared<UiConversation>();
	m_pPlayer = std::make_shared<Player>(m_pUiBar, m_pUiMain, m_modelHandle[CharacterBase::CharaType::kPlayer]);
	m_pNpc = std::make_shared<Npc>(m_modelHandle[CharacterBase::CharaType::kNpc]);
	m_pWeapon = std::make_shared<Weapon>(m_pPlayer);
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>(m_pPlayer);
	m_pEventData = std::make_shared<EventData>();

	SetShadowMap();	// シャドウマップをセットする
	SelectEnemy();	// 敵の種類を決める
	CreateSpecialEnemy(); 	// 特殊敵を生成

	// 初期化を行う
	m_pPlayer->Init();
	m_pNpc->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
	m_pUiBar->Init();
	m_pItem->Init();

	// チュートリアルが終了していない場合
	if (!m_pPlayer->GetTutoInfo().isEndTutorial)
	{
		m_isTutorial = true;
	}
}

std::shared_ptr<SceneBase> SceneMain::UpdateMenu(const Input& input)
{
	m_isPause = true;

	// ガード中の場合、ガード状態を解除する
	if (m_pPlayer->GetIsGuard())
	{
		m_pPlayer->Update(input, *m_pCamera, *m_pStage, *m_pWeapon, m_pEnemy);
	}

	// 移動中SEが再生されないようにする
	Sound::GetInstance().StopSe(SoundName::kSe_walk);
	Sound::GetInstance().StopSe(SoundName::kSe_run);

	return std::make_shared<SceneMenu>(shared_from_this(), m_pPlayer, m_pCamera);
}

std::shared_ptr<SceneBase> SceneMain::UpdateGameover()
{
	if (!m_isBattleEndStaging)
	{
		// バトル終了演出を行う
		m_isBattleEndStaging = true;
		m_battleEndStagingTime = kBattleEndStagingTime;

		for (auto& enemy : m_pEnemy)
		{
			enemy->SetIsPossibleMove(false);
		}

		return shared_from_this();
	}

	if (m_battleEndStagingTime <= 0)
	{
		return std::make_shared<SceneGameover>();
	}

	return shared_from_this();
}

void SceneMain::UpdateTutorial(const Input& input)
{
	// チュートリアルを終了する
	if (m_pPlayer->GetTutoInfo().isEndTutorial)
	{
		m_pEnemy.clear();
		m_isTutorial = false;
		return;
	}

	bool isTalk = m_pPlayer->GetTutoInfo().isTalk;
	bool isNowKnowledge = m_pPlayer->GetTutoInfo().isNowKnowledge;
	bool isCurrentTuto = m_pPlayer->GetTutoInfo().currentNum == Player::TutorialNum::kTuto_1;

	// 会話が終わったらチュートリアル敵を生成する
	if (!isTalk && !isNowKnowledge && isCurrentTuto)
	{
		// すでにクリア済みの場合は生成しない
		if (m_pPlayer->GetTutoInfo().isEndTutorial) return;

		CreateTutoEnemy();
	}

	m_nowTalkId = kTutoTalkId + std::to_string(m_pPlayer->GetTutoInfo().talkNum);
	m_pUiConversation->UpdateDispTalk(m_nowTalkId); // 会話表示を更新

	// チュートリアル状態にする
	m_pPlayer->UpdateTutorial(input, *m_pEnemy[0]);
}

void SceneMain::UpdateBattle()
{
	// 特殊敵の場合
	if (m_isLastBattle)
	{
		UpdateSpecialBattle();
		return;
	}

	// 会話中は敵を更新しない
	bool isTuto = m_pPlayer->GetTutoInfo().isNowKnowledge || m_pPlayer->GetTutoInfo().isTalk;
	if (m_pPlayer->GetIsNowTalk() || isTuto) return;

	// バトル開始演出
	UpdateBattleStartStaging();
	// バトル終了演出
	UpdateBattleEndStaging();

	// チュートリアル中
	if (m_isTutorial)
	{
		// チュートリアル敵の更新
		UpdateTutoEnemy();
		return;
	}
	else
	{
		// 敵が1体もいなくなった場合
		if (m_pEnemy.empty() && !m_isBattleEndStaging)
		{
			// バトル終了状態にする
			m_pPlayer->SetIsBattle(false);
		}

		// バトル中でない場合、敵を生成する
		if (!m_pPlayer->GetIsBattle() && m_pEnemy.empty())
		{
			CreateEnemy();
		}

		// 敵の更新
		UpdateEnemy();
	}
}

void SceneMain::UpdateSpecialBattle()
{
	// ラスボスの更新
	UpdateBossEnemy();
	// バトル開始演出
	UpdateSpecialBattleStartStaging();
	// バトル終了演出
	UpdateBattleEndStaging();
	// エンディング演出
	UpdateEndingStaging();
}

void SceneMain::UpdateSpecialBattleStartStaging()
{
	if (m_isBattleEndStaging) return;
	
	// バトル中の場合
	if (m_pPlayer->GetIsBattle())
	{
		if (!m_isDispBattleStart)
		{
			m_battleStartStagingTime = kSpecialBattleStartStagingTime;
			m_pWeapon->SetIsResetPos(true);
			m_isDispBattleStart = true;
		}

		if (m_battleStartStagingTime > 0)
		{
			m_battleStartStagingTime--;
			m_pEnemy[0]->SetIsPossibleMove(false);
			m_pPlayer->SetIsPossibleMove(false);
			m_pCamera->SetIsCameraMove(false);

			// カメラを回転させる
			m_pCamera->BattleStartProduction();
		}
		else
		{
			m_pEnemy[0]->SetIsPossibleMove(true);
			m_pPlayer->SetIsPossibleMove(true);
			m_pCamera->SetIsCameraMove(true);
		}
	}
	// バトルが終了している場合
	else
	{
		m_battleStartStagingTime = 0;
		m_isDispBattleStart = false;
	}
}

void SceneMain::UpdateBattleStartStaging()
{
	// バトル終了中はバトルを開始できないようにする
	if (m_isBattleEndStaging || m_pPlayer->GetTutoInfo().isTalk) return;

	// プレイヤーがバトル状態の場合
	if (m_pPlayer->GetIsBattle())
	{
		if (!m_isDispBattleStart)
		{
			//m_pCamera->BattleStart(); // カメラを敵の方に向ける
			m_battleStartStagingTime = kBattleStartStagingTime;
			m_pWeapon->SetIsResetPos(true);
			m_isDispBattleStart = true;
		}

		if(m_battleStartStagingTime > 0)
		{
			m_battleStartStagingTime--;

			// バトル演出中は移動できないようにする
			for (auto& enemy : m_pEnemy)
			{
				if (enemy == nullptr) continue;
				enemy->SetIsPossibleMove(false);
			}

			// 一番の近くの敵の方向を向く
			m_pPlayer->UpdateAngleNearEnemy();
		}
		else
		{
			// 移動できるようにする
			for(auto& enemy : m_pEnemy)
			{
				if (enemy == nullptr) continue;
				enemy->SetIsPossibleMove(true);
			}
			m_pPlayer->SetIsPossibleMove(true);
		}
	}
	// バトルが終了している場合
	else
	{
		m_battleStartStagingTime = 0;
		m_isDispBattleStart = false;
	}
}

std::shared_ptr<SceneBase> SceneMain::UpdateBattleEndStaging()
{
	// 演出中でない場合は飛ばす
	if (!m_isBattleEndStaging) return shared_from_this();

	// 演出中
	if (m_battleEndStagingTime > 0)
	{
		m_battleEndStagingTime--;

		// プレイヤーの終了処理
		m_pPlayer->UpdateBattleEnd();

		for (auto& enemy : m_pEnemy)
		{
			if (enemy == nullptr) continue;
			enemy->SlowAnim();
		}
	}
	// 演出終了後
	else
	{
		m_isBattleEndStaging = false;
		m_currentEnemyNum = 0;

		// エンディング中の場合は飛ばす
		if (m_isEnding) return shared_from_this();

		for (auto& enemy : m_pEnemy)
		{
			if (enemy == nullptr) continue;

			// 死亡アニメーション終了後、敵を消滅させる
			if (enemy->GetIsDead())
			{
				enemy->ResetAnim();
				enemy = nullptr;
			}
		}
		m_pPlayer->ResetAnim();
		m_pPlayer->SetIsBattle(false);
		m_pPlayer->SetIsPossibleMove(true); // プレイヤーは移動できるようにする
	}

	return shared_from_this();
}

std::shared_ptr<SceneBase> SceneMain::UpdateEndingStaging()
{
	// エンディング中でない場合は表示しない
	if(!m_isEnding) return shared_from_this();
	// バトル終了演出中は飛ばす
	if (m_isBattleEndStaging) return shared_from_this();

	// 演出中
	if (m_endingTime > 0)
	{
		m_endingTime--;
	}
	// 演出終了後
	else
	{
		// エンディング終了後、クリア画面に遷移する
		return std::make_shared<SceneClear>();
	}

	return shared_from_this();
}

void SceneMain::UpdateSound()
{
	auto& sound = Sound::GetInstance();

	// バトル終了演出中
	if (m_battleEndStagingTime > 0 && m_isBattleEndStaging)
	{
		sound.StopBgm(SoundName::kBgm_battle);

		// 終了BGMを流す
		if (m_pPlayer->GetHp() <= 0.0f) return;
		Sound::GetInstance().PlayBgm(SoundName::kBgm_battleEnd);
	}
	// バトル中BGMを再生する
	else if (m_pPlayer->GetIsBattle())
	{
		sound.StopBgm(SoundName::kBgm_congestion);

		// ボス戦の場合
		if (m_isLastBattle)
		{
			sound.PlayLoopBgm(SoundName::kBgm_bossBattle);

			if (m_pEnemy[0] == nullptr) return;
			if (m_pEnemy[0]->GetHp() <= 0.0f)
			{
				sound.StopBgm(SoundName::kBgm_bossBattle);
			}
		}
		// ボス戦以外の場合
		else
		{
			
			sound.PlayLoopBgm(SoundName::kBgm_battle);
		}
	}
	else
	{
		sound.StopBgm(SoundName::kBgm_battle);
		sound.PlayLoopBgm(SoundName::kBgm_congestion);
	}
}

void SceneMain::CreateEnemy()
{
	// チュートリアル前は生成しない
	if (!m_pPlayer->GetTutoInfo().isEndTutorial && m_isTutorial) return;
	// ゲーム開始時、時間が経ってから敵を生成する
	if (m_mainSceneTime <= kFirstSpawnTime + kLoadingTime) return;
	// 会話中は敵を生成しない
	if (m_pPlayer->GetIsNowTalk()) return;

	// スポーンするまでの時間をランダムで決める
	const int spawnTime = GetRand(kEnemySpawnMaxTime) + kEnemySpawnMinTime;
	m_enemySpawnTime++;

	if (m_enemySpawnTime >= spawnTime)
	{
		SelectEnemy(); // 出現する敵をランダムで選ぶ
		m_enemySpawnTime = 0;
	}
}

void SceneMain::CreateSpecialEnemy()
{
	for (int i = 0; i < m_pEnemySpecial.size(); i++)
	{
		int index = i + kSpecialEnemyAddIndex; // 特殊敵の番号を調整

		m_pEnemySpecial[i] = std::make_shared<EnemySpecial>(m_pUiBar, m_pItem, *m_pPlayer);
		m_pEnemySpecial[i]->SetEnemyInfo(kEnemyName.at(index), kSpecialEnemyId.at(index),
			index, m_modelHandle[index]);
		m_pEnemySpecial[i]->Init();
	}
}

void SceneMain::CreateTutoEnemy()
{
	// すでにクリア済みの場合は生成しない
	if (m_pPlayer->GetTutoInfo().isEndTutorial) return;
	if (m_isCreateTutoEnemy) return;

	m_isCreateTutoEnemy = true;

	m_currentEnemyNum = 1;
	m_pPlayer->SetIsBattle(true);

	// 敵がいる場合は削除する
	m_pEnemy.clear();

	// チュートリアル用の敵を生成する
	int enemyIndex = CharacterBase::CharaType::kEnemy_tuto;

	auto tutoEnemy = std::make_shared<EnemyBase>(m_pUiBar, m_pItem, *m_pPlayer);
	tutoEnemy->SetEnemyInfo(kEnemyName.at(enemyIndex), "enemy_tuto", enemyIndex, m_modelHandle[enemyIndex]);
	tutoEnemy->SetEnemySpawnPos(*m_pPlayer, 0);
	tutoEnemy->Init();
	m_pEnemy.push_back(tutoEnemy);
	tutoEnemy->GetEnemyAI()->SetEnemyList(m_pEnemy);
	return;
}

void SceneMain::CreateBossEnemy()
{
	// 敵がいる場合は削除する
	m_pEnemy.clear();

	m_currentEnemyNum = 1;

	// ラスボス用の敵を生成する
	int enemyIndex = CharacterBase::CharaType::kEnemy_boss;

	auto bossEnemy = std::make_shared<EnemyBase>(m_pUiBar, m_pItem, *m_pPlayer);
	bossEnemy->SetEnemyInfo(kEnemyName.at(enemyIndex), "enemy_boss", enemyIndex, m_modelHandle[enemyIndex]);
	bossEnemy->SetEnemySpawnPos(*m_pPlayer, 0);
	bossEnemy->Init();
	m_pEnemy.push_back(bossEnemy);
	bossEnemy->GetEnemyAI()->SetEnemyList(m_pEnemy);
}

void SceneMain::UpdateEnemy()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		if (m_pEnemy[i] == nullptr) continue;

		// 残り1体になった場合
		if (m_currentEnemyNum == 1)
		{
			// 敵のHPが0以下になった場合
			if (!m_isBattleEndStaging && m_pEnemy[i]->GetHp() <= 0.0f)
			{
				// 攻撃アニメーション中が終わるまでバトル終了演出を行わない
				bool isAttack = m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk1 || m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk2 || m_pPlayer->GetIsAttack();
				if (isAttack) return;

				// バトル終了演出を行う
				m_isBattleEndStaging = true;
				m_battleEndStagingTime = kBattleEndStagingTime;
				return;
			}
		}

		// 特定の状態の場合、敵を消滅させる
		if (IsExtinction(i))
		{
			m_pEnemy[i] = nullptr;

			// バトル中の場合は、倒した敵数を増やす
			if (m_pPlayer->GetIsBattle())
			{
				m_pPlayer->AddDeadEnemyNum();
				m_currentEnemyNum--;
			}
		}
		else
		{
			m_pEnemy[i]->Update(*m_pStage, *m_pPlayer);
		}

		// 敵同士の当たり判定をチェックする
		for (int j = 0; j < m_pEnemy.size(); j++)
		{
			if ((i == j) || (m_pEnemy[i] == nullptr) || (m_pEnemy[j] == nullptr)) continue;
			m_pEnemy[i]->CheckCharaCol(*m_pEnemy[j], m_pEnemy[j]->GetCol(j), j);
		}

		m_pEnemy.erase(std::remove(m_pEnemy.begin(), m_pEnemy.end(), nullptr), m_pEnemy.end());
	}
}

void SceneMain::UpdateTutoEnemy()
{
	if (m_pEnemy[0] == nullptr) return;

	// 途中まで回復させる
	if (!m_pPlayer->GetTutoInfo().isEndTutorial && m_pPlayer->GetTutoInfo().currentNum <= Player::TutorialNum::kTuto_4)
	{
		m_pEnemy[0]->RecoveryHp();
	}
	
	// HPが0になった場合
	if (!m_isBattleEndStaging && m_pEnemy[0]->GetHp() <= 0.0f)
	{
		// 攻撃アニメーション中が終わるまでバトル終了演出を行わない
		bool isAttack = m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk1 || m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk2 || m_pPlayer->GetIsAttack();
		if (isAttack) return;

		// バトル終了演出を行う
		m_isBattleEndStaging = true;
		m_battleEndStagingTime = kBattleEndStagingTime;
		return;
	}

	// 特定の状態の場合、敵を消滅させる
	if (IsExtinction(0))
	{
		m_pEnemy[0] = nullptr;

		// 倒した敵数を増やす
		if (m_pPlayer->GetIsBattle())
		{
			m_pPlayer->AddDeadEnemyNum();
			m_currentEnemyNum--;
		}
	}
	else
	{
		// 更新
		m_pEnemy[0]->Update(*m_pStage, *m_pPlayer);
	}
}

void SceneMain::UpdateBossEnemy()
{
	if (m_pEnemy[0] == nullptr) return;

	// HPが0になった場合
	if (!m_isBattleEndStaging && m_pEnemy[0]->GetHp() <= 0.0f)
	{
		// 攻撃アニメーション中が終わるまでバトル終了演出を行わない
		bool isAttack = m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk1 || m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk2 || m_pPlayer->GetIsAttack();
		if (isAttack) return;

		// バトル終了演出を行う
		m_isBattleEndStaging = true;
		m_isEnding = true;
		m_battleEndStagingTime = kBattleEndStagingTime;
		return;
	}

	// 特定の状態の場合、敵を消滅させる
	if (IsExtinction(0))
	{
		m_pEnemy[0] = nullptr;

		// バトル中の場合は、倒した敵数を増やす
		if (m_pPlayer->GetIsBattle())
		{
			m_pPlayer->AddDeadEnemyNum();
			m_currentEnemyNum--;
		}
	}
	else
	{
		// 更新
		m_pEnemy[0]->Update(*m_pStage, *m_pPlayer);
	}

	m_pEnemy.erase(std::remove(m_pEnemy.begin(), m_pEnemy.end(), nullptr), m_pEnemy.end());
}

void SceneMain::SelectEnemy()
{
	LoadCsv::GetInstance().LoadEnemyName(); // 敵名を読み込む

	// 出現する敵の数をランダムで決定する
	int enemySpawnNum = GetRand(kEnemyMaxNum - 1) + 1;
	m_pEnemy.clear();
	m_pEnemy.resize(enemySpawnNum);
	m_currentEnemyNum = enemySpawnNum;

	std::unordered_set<int> enemyKind;  // 生成された敵の種類を保持する
	std::unordered_set<int> enemyName;  // 決まった敵名を保持する

	std::random_device rd;
	std::mt19937 mt(rd());

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 敵の種類が重複しないようにする
		int enemyIndex;
		do
		{
			std::uniform_int_distribution urdIndex(1, kEnemyKindNum);
			enemyIndex = urdIndex(mt);
		} while (enemyKind.count(enemyIndex) > 0);	 // MEMO:countは要素が見つかったら1を、見つからない場合は0を返す

		enemyKind.insert(enemyIndex);

		// 敵名が重複しないようにする
		int enemyNameIndex;
		do
		{
			// 敵の名前をランダムで選ぶ
			std::uniform_int_distribution urdIndex(1, kEnemyNamekind);
			enemyNameIndex = urdIndex(mt);
		} while (enemyName.count(enemyNameIndex) > 0);

		enemyName.insert(enemyNameIndex);
		std::string enemyName = LoadCsv::GetInstance().GetEnemyName(enemyNameIndex);
		
		// 2桁にそろえる
		char enemyId[3];
		sprintf_s(enemyId, "%02d", enemyIndex);

		// MEMO:すでに読み込んだモデルをコピーしないとアニメーションがおかしくなる
		m_modelHandle[enemyIndex] = MV1DuplicateModel(m_modelHandle[(enemyIndex)]);

		m_pEnemy[i] = std::make_shared<EnemyBase>(m_pUiBar, m_pItem, *m_pPlayer);
		m_pEnemy[i]->SetEnemyInfo(enemyName, "enemy_" + std::string(enemyId), enemyIndex, m_modelHandle[enemyIndex]);
		m_pEnemy[i]->SetEnemySpawnPos(*m_pPlayer, i);
		m_pEnemy[i]->Init();
		m_pEnemy[i]->GetEnemyAI()->SetEnemyList(m_pEnemy);
	}
}

bool SceneMain::IsExtinction(int index)
{
	// 敵が死亡した場合
	if (m_pEnemy[index]->GetIsDead()) return true;

	// 敵が地面の下に落ちた場合
	if (m_pEnemy[index]->GetPos().y <= 0.0f) return true;

	// 特定の敵の場合は飛ばす
	bool isTuto = m_pEnemy[index]->GetEnemyIndex() == CharacterBase::CharaType::kEnemy_tuto;
	bool isBoss = m_pEnemy[index]->GetEnemyIndex() == CharacterBase::CharaType::kEnemy_boss;
	if (isTuto || isBoss) return false;

	// プレイヤーと敵の距離を求める
	float pToEDist = VSize(VSub(m_pPlayer->GetPos(), m_pEnemy[index]->GetPos()));
	// 敵がプレイヤーから一定距離離れた場合
	if(pToEDist >= kEnemyExtinctionDist) return true;

	return false;
}

void SceneMain::CheckEventTrigger(const Input& input)
{
	// プレイヤー座標を取得
	VECTOR playerPos = m_pPlayer->GetPos();

	// イベント用の当たり判定を取得
	const auto& eventData = m_pEventData->GetEventData();
	for (const auto& data : eventData)
	{
		// 特定の場合は飛ばす
		if (m_pPlayer->GetIsNowTalk() || m_pPlayer->GetIsBattle() || m_isLastBattle) return;

		// プレイヤーの当たり判定を取得する
		auto playerCol = m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer);

		// イベント用カプセルとプレイヤーの当たり判定をチェックする
		bool isCol = HitCheck_Sphere_Capsule(data.pos, data.radius, playerCol.bodyUpdateStartPos, playerCol.bodyUpdateEndPos, playerCol.bodyRadius);

		// 当たっている場合
		if (isCol)
		{
			// イベントIDに応じた処理を行う
			StartEvent(data.eventId, input);
		}
		else
		{
			m_pPlayer->SetIsTalk(false);
		}
	}
}

void SceneMain::StartEvent(const std::string& eventId, const Input& input)
{
	// IDに応じて処理を変更する
	if (eventId == "TALK_NPC")
	{
		// 会話できる状態にする
		m_pPlayer->SetIsTalk(true);

		// 決定ボタンを押した場合
		if (input.IsTriggered(InputId::kOk))
		{
			// 会話中にする
			m_pPlayer->SetIsNowTalk(true);
			m_pPlayer->SetIsTalk(false);
			m_isDispTalkSelect = true;
			m_talkDispTime = kTalkDispTime;
			m_nowTalkId = ConversationID::kTalkStart;
		}
	}
}

void SceneMain::UpdateTalk(const Input& input)
{
	// 特定の状態中は会話できないようにする
	if (!m_pPlayer->GetIsNowTalk() || m_pPlayer->GetIsBattle() || m_isBattleEndStaging) return;

	m_pUiConversation->UpdateDispTalk(m_nowTalkId); // 会話表示を更新

	m_talkDispTime--;
	if (m_talkDispTime > 0) return;

	// 会話中
	if (m_isTalking && !m_isChoice)
	{
		if (input.IsTriggered(InputId::kOk))
		{
			m_pUiConversation->ResetDispTalk(); // 会話表示をリセットする
			EndTalk(); // 会話を終了する
			return;
		}
	}

	// 選択肢カーソル更新
	if (m_isDispTalkSelect)
	{
		UpdateTalkSelect(input);
		m_pUi->UpdateCursor(kCursorId);
	}

	// 2択表示中
	if (m_isChoice)
	{
		// 選択状態を更新
		UpdateChoice(input);
		m_pUi->UpdateCursor(kChoiceCursorId);

		if (input.IsTriggered(InputId::kA))
		{
			// 選択肢を決める
			DecideChoice();
			return;
		}

		if (input.IsTriggered(InputId::kBack))
		{
			m_isChoice = false;
			m_pUiConversation->ResetDispTalk(); // 会話表示をリセットする
			EndTalk(); // 会話を終了する
		}
	}

	// Bボタンを押した場合
	if (input.IsTriggered(InputId::kBack))
	{
		m_pUiConversation->ResetDispTalk(); // 会話表示をリセットする
		EndTalk(); // 会話を終了する
		return;
	}

	// 選択肢決定
	if (input.IsTriggered(InputId::kOk))
	{
		m_isTalking = true;
		m_isDispTalkSelect = false;

		// 選択した状態に更新
		switch (m_talkSelect)
		{
		case TalkSelect::kBattle:
			SelectBattle(input);
			break;
		case TalkSelect::kDeadEnemyNum:
			SelectDeadEnemyNum(input);
			break;
		case TalkSelect::kRecovery:
			SelectRecovery(input);
			break;
		case TalkSelect::kGetItem:
			SelectGetItem(input);
			break;
		case TalkSelect::kBack:
			EndTalk();
			break;
		}
	}
}

void SceneMain::UpdateTalkSelect(const Input& input)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered(InputId::kDown))
	{
		m_talkSelect = (m_talkSelect + 1) % TalkSelect::kTalkNum;
		m_pUi->Init();

		Sound::GetInstance().PlayBackSe(SoundName::kSe_cursor);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_talkSelect = (m_talkSelect + (TalkSelect::kTalkNum - 1)) % TalkSelect::kTalkNum;
		m_pUi->Init();

		Sound::GetInstance().PlayBackSe(SoundName::kSe_cursor);
	}
}

void SceneMain::DecideChoice()
{
	switch (m_talkSelect)
	{
	case TalkSelect::kBattle:
		// 「戦いに往く」を選んだ場合
		if (m_choiceSelect == Choice::kYes)
		{
			// ボスと戦う条件を満たしている場合
			if (m_pPlayer->GetDeadEnemyNum() >= kClearEnemyNum)
			{
				// ボス戦開始
				m_nowTalkId = ConversationID::kBattleOk;
				m_pItem->DeleteDropItem(); // ドロップアイテムを削除
				//EffectManager::GetInstance().AllStop(); // 全エフェクトを停止する
				m_isLastBattle = true;
				m_isLoading = true;
				EndTalk();
			}
			else
			{
				m_nowTalkId = ConversationID::kBattleNg;
			}
		}
		// 「やめる」を選んだ場合
		else
		{
			EndTalk();
		}
		break;

	case TalkSelect::kRecovery:
		// 「回復する」を選んだ場合
		if (m_choiceSelect == Choice::kYes)
		{
			// 金額が足りない場合
			if (m_pPlayer->GetMoney() < kRecoveryMoney)
			{
				m_nowTalkId = ConversationID::kRecoveryNg;
				return;
			}

			// HPが最大の場合
			if (m_pPlayer->GetHp() >= m_pPlayer->GetStatus().maxHp && m_pPlayer->GetGauge() >= m_pPlayer->GetStatus().maxGauge)
			{
				m_nowTalkId = ConversationID::kRecoveryMax;
				return;
			}

			m_nowTalkId = ConversationID::kRecoveryOk;
			Sound::GetInstance().PlaySe(SoundName::kSe_recovery);
			m_pPlayer->RecoveryHp(kRecoveryMaxRate);
			m_pPlayer->RecoveryGauge(kRecoveryMaxRate);
			m_pPlayer->AddDecreaseMoney(-kRecoveryMoney);
		}
		// 「回復しない」を選んだ場合
		else
		{
			EndTalk();
		}
		break;

	case TalkSelect::kGetItem:
		// 「アイテム購入」を選んだ場合
		if (m_choiceSelect == Choice::kYes)
		{
			if (m_pPlayer->GetIsAddItem())
			{
				if (m_pPlayer->GetMoney() < kGetItemMoney)
				{
					m_nowTalkId = ConversationID::kGetItemNg_money;
					return;
				}

				m_nowTalkId = ConversationID::kGetItemOk;
				Sound::GetInstance().PlaySe(SoundName::kSe_getItem);
				m_pPlayer->AddItem(GetRand(Item::ItemType::kItemKind));
				m_pPlayer->AddDecreaseMoney(-kGetItemMoney);
			}
			else
			{
				m_nowTalkId = ConversationID::kGetItemNg_itemMax;
			}
		}
		// 「やめる」を選んだ場合
		else
		{
			EndTalk();
		}
		break;

	default:
		break;
	}

	m_isChoice = false; // 選択状態を解除
}

void SceneMain::SelectBattle(const Input& input)
{
	m_isChoice = true;
	m_isDispTalkSelect = false;
}

void SceneMain::SelectDeadEnemyNum(const Input& input)
{
	m_nowTalkId = ConversationID::kDeadNum;
}

void SceneMain::SelectRecovery(const Input& input)
{
	m_nowTalkId = ConversationID::kRecovery;
	m_isChoice = true;
	m_isDispTalkSelect = false;
}

void SceneMain::SelectGetItem(const Input& input)
{
	m_nowTalkId = ConversationID::kGetItem;
	m_isChoice = true;
	m_isDispTalkSelect = false;
}

void SceneMain::EndTalk()
{
	// 会話状態を解除する
	m_pPlayer->SetIsNowTalk(false);
	m_pPlayer->SetIsPossibleMove(true);
	m_talkSelect = TalkSelect::kBattle;
	m_choiceSelect = Choice::kYes;
	m_nowTalkId = ConversationID::kNone;
	m_pUiConversation->UpdateDispTalk(m_nowTalkId); // 会話表示を更新
	m_isTalking = false;
	m_isChoice = false;
}

void SceneMain::DrawTalk()
{
	// チュートリアル中は表示しない
	if (m_isTutorial) return;
	if (m_pPlayer->GetIsBattle()) return;

	// 話すUI表示
	if (m_pPlayer->GetIsTalk())
	{
		m_pUiMain->DrawNpcUi(m_pNpc->GetPos());
	}
	else if(!m_pPlayer->GetIsTalk())
	{
		m_pUiMain->DrawTalkIcon(m_pPlayer->GetPos(), m_pNpc->GetPos());
	}

	// 会話中
	if (m_pPlayer->GetIsNowTalk())
	{
		m_pUiConversation->DrawTalk(*m_pPlayer, kClearEnemyNum); // 会話画面

		// 2択表示
		if (m_isChoice)
		{
			m_pUiConversation->DrawTalkSelectBg(); // 選択肢の背景
			m_pUi->DrawCursor(kChoiceCursorId, m_choiceSelect, kChoiceCursorInterval);	// 選択カーソル
			m_pUiConversation->DrawChoice(m_talkSelect); // 選択肢テキスト
		}
		// 選択肢表示
		else if (m_isDispTalkSelect)
		{
			m_pUiConversation->DrawTalkSelectBg(); // 選択肢の背景
			m_pUi->DrawCursor(kCursorId, m_talkSelect, kTalkSelectCursorInterval);	// 選択カーソル
			m_pUiConversation->DrawTalkSelectText(); // 選択肢テキスト
		}
	}
}

void SceneMain::DrawExplain()
{
	// チュートリアル表示
	if (m_isTutorial)
	{
		if (m_pPlayer->GetTutoInfo().isEndTutorial) return;
		m_pUiMain->DrawTutorial(m_pPlayer->GetTutoInfo());

		// 会話表示
		if (m_pPlayer->GetTutoInfo().isTalk)
		{
			m_pUiConversation->DrawTalk(*m_pPlayer, 0);
		}
	}
	// 操作説明表示
	else
	{
		// 会話中は操作説明を表示しない
		if (!m_pPlayer->GetIsNowTalk())
		{
			m_pUiMain->DrawOperation(m_pPlayer->GetIsBattle());
		}
	}
}

void SceneMain::SetShadowMap()
{
	m_shadowMap = MakeShadowMap(kShadowMapSize, kShadowMapSize);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMap, kShadowAreaMinPos, kShadowAreaMaxPos);
	// シャドウマップが想定するライトの方向をセット
	SetShadowMapLightDirection(m_shadowMap, kShadowDir);
	
	// 影色を調整
	SetLightAmbColor(GetColorF(0.7f, 0.7f, 0.7f, 0.0f));
}

void SceneMain::DrawSetUpShadow()
{
	// ステージモデル用のシャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMap);
	m_pStage->DrawStage();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pNpc->Draw();

	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}

	for (auto& enemy : m_pEnemySpecial)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}

	m_pWeapon->Draw();
	ShadowMap_DrawEnd(); /*シャドウマップへの描画を終了*/

	/*描画に使用するシャドウマップを設定*/
	SetUseShadowMap(0, m_shadowMap);
	m_pStage->DrawStage();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pNpc->Draw();

	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}

	for (auto& enemy : m_pEnemySpecial)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}

	m_pWeapon->Draw();
	SetUseShadowMap(0, -1); /*描画に使用するシャドウマップの設定を解除*/
}
