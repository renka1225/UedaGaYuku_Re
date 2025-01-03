#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Sound.h"
#include "EffectManager.h"
#include "UiBar.h"
#include "Camera.h"
#include "CharacterBase.h"
#include "Player.h"
#include "EnemyBase.h"
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
	const std::string kPlayerHandlePath = "data/model/chara/player.mv1"; // プレイヤーのモデルハンドルパス
	const std::string kEnemyHandlePath = "data/model/chara/enemy_";		 // 敵のモデルハンドルパス

	constexpr int kModelNum = 4;		// 読み込むモデルの数
	constexpr int kEnemyMaxNum = 2;		// 1度に出現する最大の敵数
	constexpr int kEnemyKindNum = 2;	// 敵の種類
	constexpr int kEnemyNamekind = 31;	// 敵名の種類
	constexpr int kClearEnemyNum = 3;	// クリア条件
	constexpr int kEnemySpawnMinTIme = 300;			// 敵がスポーンするまでの最小時間
	constexpr int kEnemySpawnMaxTIme = 3000;		// 敵がスポーンするまでの最大時間
	constexpr float kEnemyExtinctionDist = 2000.0f;	// 敵が消滅する範囲

	constexpr int kBattleStartStagingTime = 120; // バトル開始時の演出時間
	constexpr int kBattleEndStagingTime = 150;	 // バトル終了時の演出時間
	constexpr int kEndingTime = 60;				 // エンディングの時間
}

SceneMain::SceneMain():
	m_currentEnemyNum(0),
	m_enemySpawnTime(0),
	m_battleStartStagingTime(0),
	m_battleEndStagingTime(0),
	m_endingTime(0),
	m_isDispBattleStart(false),
	m_isBattleEndStaging(false),
	m_isEnding(false),
	m_isPause(false),
	m_isLoading(true),
	m_isLastBattle(false)
{
	// 非同期読み込み設定に変更
	SetUseASyncLoadFlag(true);

	m_modelHandle.resize(kModelNum);
	m_pEnemy.resize(kEnemyKindNum);

	LoadModelHandle(); // モデルを読み込む
}

SceneMain::~SceneMain()
{
	// サウンド停止
	Sound::GetInstance().StopBgm(SoundName::kBgm_congestion);
	Sound::GetInstance().StopBgm(SoundName::kBgm_battle);
	Sound::GetInstance().StopBgm(SoundName::kBgm_bossBattle);
	Sound::GetInstance().StopBgm(SoundName::kBgm_battleEnd);

	m_pEnemy.clear();
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
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
	// ロード中の場合
	if (m_isLoading)
	{
		Loading();
		return shared_from_this();
	}

	// エンディング中の場合
	if (m_isEnding)
	{
		auto nextScene = UpdateEndingStaging();
		if (nextScene != shared_from_this()) return nextScene;
		return shared_from_this();
	}

	// メニューを開いたとき
	if (input.IsTriggered(InputId::kMenu))
	{
		m_isPause = true;
		// 移動中SEが再生されないようにする
		Sound::GetInstance().StopSe(SoundName::kSe_walk);
		Sound::GetInstance().StopSe(SoundName::kSe_run);
		return std::make_shared<SceneMenu>(shared_from_this(), m_pPlayer, m_pCamera);
	}

	// ゲームオーバー
	if (m_pPlayer->GetIsDead() && !m_isBattleEndStaging)
	{
		// バトル終了演出を行う
		m_isBattleEndStaging = true;
		m_battleEndStagingTime = kBattleEndStagingTime;
		shared_from_this();
	}

	// 最終決戦中でない場合
	if (!m_isLastBattle)
	{
		// バトル開始演出
		UpdateBattleStartStaging();

		// 敵が1体もいなくなった場合
		if (m_pEnemy.empty() && !m_isBattleEndStaging)
		{
			// バトル終了状態にする
			m_pPlayer->SetIsBattle(false);
		}

		// バトル中でない場合、敵を生成する
		if (!m_pPlayer->GetIsBattle())
		{
			CreateEnemy();
		}
		
		// 敵の更新
		UpdateEnemy();
	}
	else
	{
		// ラスボスの更新
		UpdateBossEnemy();
		// エンディング演出
		UpdateEndingStaging();
	}
	// バトル終了演出
	UpdateBattleEndStaging();

	m_pPlayer->Update(input, *m_pCamera, *m_pStage, *m_pWeapon, m_pEnemy);
	m_pItem->Update(*m_pPlayer);
	m_pWeapon->Update(*m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);
	m_pUiBar->Update();

	// イベントトリガーのチェック
	CheckEventTrigger();

	// エフェクトの更新
	EffectManager::GetInstance().Update();

	// サウンドの更新
	UpdateSound();

#ifdef _DEBUG // デバックコマンド
	if (input.IsTriggered(InputId::kDebugClear))
	{
		return std::make_shared<SceneClear>();
	}
	else if (input.IsTriggered(InputId::kDebugEnding))
	{
		m_isEnding = true;
	}
	else if (input.IsTriggered(InputId::kDebugGameover))
	{
		return std::make_shared<SceneGameover>();
	}
#endif

	return shared_from_this();
}

void SceneMain::Draw()
{
	// ロード中
	if (m_isLoading)
	{
		m_pUi->DrawLoading();
		return;
	}

	// エンディング演出を表示
	if (m_isEnding)
	{
		m_pUi->DrawEnding();
		return;
	}

	m_pStage->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();

	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}

	m_pWeapon->Draw();
	EffectManager::GetInstance().Draw();

	// 最終決戦でない場合
	if (!m_isLastBattle)
	{
		// バトル開始の演出を表示
		if (m_battleStartStagingTime > 0)
		{
			m_pUi->DrawBattleStart();
		}
		// バトル終了の演出を表示
		if (m_battleEndStagingTime > 0)
		{
			m_pUi->DrawBattleEnd();
		}

		// バトル中UI表示
		m_pUi->DrawBattleUi(*m_pPlayer);
	}

	// プレイヤーのバーUI表示
	m_pUiBar->DrawPlayerHpBar(*m_pPlayer, m_pPlayer->GetStatus().maxHp);
	m_pUiBar->DrawPlayerGaugeBar(*m_pPlayer, m_pPlayer->GetStatus().maxGauge);

	// 特定の状態の場合は表示しない
	bool isDrawMap = !m_isBattleEndStaging || !m_isLastBattle || m_isEnding;
	if (isDrawMap)
	{
		// ミニマップを表示
		m_pUi->DrawMiniMap(*m_pPlayer, m_pEnemy);
	}
	
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
	m_pEventData->Draw();
	DrawFormatString(0, 550, Color::kColorW, "倒した敵数:%d\n", m_pPlayer->GetDeadEnemyNum());
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
		char enemyId[3];
		sprintf_s(enemyId, "%02d", (i + 1));
		m_modelHandle[(i + 1)] = MV1LoadModel((kEnemyHandlePath + std::string(enemyId) + ".mv1").c_str());
	}

	// ラスボス
	m_modelHandle[CharacterBase::CharaType::kEnemy_boss] = MV1LoadModel((kEnemyHandlePath + "boss.mv1").c_str());
}

void SceneMain::Loading()
{
	// 非同期読み込み数を確認
	int aSyncLoadNum = GetASyncLoadNum();

	// すべての読み込みが終了した場合
	if (aSyncLoadNum == 0)
	{
		// 同期読み込み設定に変更
		SetUseASyncLoadFlag(false);
		m_isLoading = false;

		// ロード完了後の処理を行う
		InitAfterLoading();
	}
}

void SceneMain::InitAfterLoading()
{
	m_pUiBar = std::make_shared<UiBar>();
	m_pPlayer = std::make_shared<Player>(m_pUiBar, m_modelHandle[CharacterBase::CharaType::kPlayer]);
	m_pWeapon = std::make_shared<Weapon>(m_pPlayer);
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>(m_pPlayer);
	m_pEventData = std::make_shared<EventData>();

	SelectEnemy(); // 敵の種類を決める

	// 初期化を行う
	m_pPlayer->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
	m_pUiBar->Init();
	m_pItem->Init();
}

void SceneMain::UpdateBattleStartStaging()
{
	// プレイヤーがバトル状態の場合
	if (m_pPlayer->GetIsBattle())
	{
		if (!m_isDispBattleStart)
		{
			m_battleStartStagingTime = kBattleStartStagingTime;
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
			m_pPlayer->SetIsPossibleMove(false);
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

		for (auto& enemy : m_pEnemy)
		{
			if (enemy == nullptr) continue;

			// アニメーションをスローで再生する
			enemy->SlowAnim();
		}

		// 演出中は移動できないようにする
		m_pPlayer->SetIsPossibleMove(false);
		// アニメーションをスローで再生する
		m_pPlayer->SlowAnim();
	}
	// 演出終了後
	else
	{
		// プレイヤーが死亡している場合、ゲームオーバー画面に遷移する
		if (m_pPlayer->GetIsDead()) return std::make_shared<SceneGameover>();

		m_isBattleEndStaging = false;
		m_currentEnemyNum = 0;

		for (auto& enemy : m_pEnemy)
		{
			if (enemy == nullptr) continue;

			// 死亡アニメーション終了後、敵を消滅させる
			if (enemy->GetIsDead()) enemy = nullptr;

			enemy->ResetAnim();
		}

		// プレイヤーは移動できるようにする
		m_pPlayer->SetIsPossibleMove(true);
		m_pPlayer->ResetAnim();
	}

	return shared_from_this();
}

std::shared_ptr<SceneBase> SceneMain::UpdateEndingStaging()
{
	// エンディング中でない場合は表示しない
	if(!m_isEnding && m_isBattleEndStaging) return shared_from_this();

	// 演出中
	if (m_endingTime > 0)
	{
		m_endingTime--;
		printfDx("エンディング中\n");
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

	if (m_battleEndStagingTime > 0)
	{
		// 終了BGMを流す
		Sound::GetInstance().PlayBgm(SoundName::kBgm_battleEnd);
		sound.StopBgm(SoundName::kBgm_battle);
	}
	// バトル中BGMを再生する
	else if (m_pPlayer->GetIsBattle())
	{
		sound.StopBgm(SoundName::kBgm_congestion);

		// ボス戦の場合
		if (m_isLastBattle)
		{
			sound.PlayLoopBgm(SoundName::kBgm_bossBattle);

			if (m_pEnemy[0]->GetHp() < 0.0f)
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
	// ラスボス戦でない場合
	if (!m_isLastBattle)
	{
		// スポーンするまでの時間をランダムで決める
		const int spawnTime = GetRand(kEnemySpawnMaxTIme) + kEnemySpawnMinTIme;
		m_enemySpawnTime++;

		if (m_enemySpawnTime >= spawnTime)
		{
			m_enemySpawnTime = 0;
			// 出現する敵をランダムで選ぶ
			SelectEnemy();
		}
	}
	// ラスボス戦の場合
	else
	{
		// 敵がいる場合は削除する
		m_pEnemy.clear();

		// ラスボス用の敵を生成する
		int enemyIndex = CharacterBase::CharaType::kEnemy_boss;
		
		auto bossEnemy = std::make_shared<EnemyBase>(m_pUiBar, m_pItem, *m_pPlayer);
		bossEnemy->SetEnemyInfo("ラスボス", "enemy_boss", enemyIndex, m_modelHandle[enemyIndex]);
		bossEnemy->SetEnemySpawnPos(*m_pPlayer, 0);
		bossEnemy->Init();
		m_pEnemy.push_back(bossEnemy);
	}
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
			m_pPlayer->AddDeadEnemyNum();
			m_currentEnemyNum--;
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

void SceneMain::UpdateBossEnemy()
{
	// ラスボスの更新
	m_pEnemy[0]->Update(*m_pStage, *m_pPlayer);

	// HPが0になった場合
	if (m_pEnemy[0]->GetHp() <= 0.0f)
	{
		// クリア演出を行う
		// TODO:エンディングを流す
		m_endingTime = kEndingTime;
		m_isEnding = true;
	}
}

void SceneMain::SelectEnemy()
{
	LoadCsv::GetInstance().LoadEnemyName(); // 敵名を読み込む

	// 出現する敵の数をランダムで決定する
	int enemySpawnNum = GetRand(kEnemyMaxNum - 1) + 1;
	//int enemySpawnNum = 2;
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

	// プレイヤーと敵の距離を求める
	float pToEDist = VSize(VSub(m_pPlayer->GetPos(), m_pEnemy[index]->GetPos()));
	// 敵がプレイヤーから一定距離離れた場合
	if(pToEDist >= kEnemyExtinctionDist) return true;

	return false;
}

void SceneMain::CheckEventTrigger()
{
	// プレイヤー座標を取得
	VECTOR playerPos = m_pPlayer->GetPos();

	// イベント用の当たり判定を取得
	const auto& eventData = m_pEventData->GetEventData();
	for (const auto& event : eventData)
	{
		// プレイヤーの当たり判定を取得する
		auto playerCol = m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer);

		// イベント用カプセルとプレイヤーの当たり判定をチェックする
		bool isCol = HitCheck_Sphere_Capsule(event.pos, event.radius, playerCol.bodyUpdateStartPos, playerCol.bodyUpdateEndPos, playerCol.bodyRadius);

		// 当たっている場合かつバトル中でない場合
		if (isCol && !m_pPlayer->GetIsBattle())
		{
			// イベントIDに応じた処理を行う
			StartEvent(event.eventId);
		}
	}
}

void SceneMain::StartEvent(const std::string& eventId)
{
	if (m_isLastBattle) return;

	// IDに応じて処理を変更する
	if (eventId == "bossBattle")
	{
		if (m_pPlayer->GetDeadEnemyNum() < kClearEnemyNum) return;
		printfDx("ボスバトル開始\n");
		m_isLastBattle = true;
		CreateEnemy();
		m_pPlayer->SetIsBattle(true);
	}
}
