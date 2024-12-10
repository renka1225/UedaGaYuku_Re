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
#include "ObjectBase.h"
#include "Weapon.h"
#include "Item.h"
#include "Stage.h"
#include "EventData.h"
#include "SceneMenu.h"
#include "SceneMain.h"
#include <unordered_set>
#include <random>
#include <windows.h>

// 定数
namespace
{
	const char* kPlayerHandlePath = "data/model/chara/player.mv1";	// プレイヤーのモデルハンドルパス
	const char* kEnemyHandlePath = "data/model/chara/enemy_";		// 敵のモデルハンドルパス

	constexpr int kModelNum = 3;		// 読み込むモデルの数
	constexpr int kEnemyMaxNum = 2;		// 1度に出現する最大の敵数
	constexpr int kEnemyKindNum = 2;	// 敵の種類
	constexpr int kEnemyNamekind = 31;	// 敵名の種類
	constexpr int kEnemySpawnMinTIme = 180;			// 敵がスポーンするまでの最小時間
	constexpr int kEnemySpawnMaxTIme = 3000;		// 敵がスポーンするまでの最大時間
	constexpr float kEnemyExtinctionDist = 2000.0f;	// 敵が消滅する範囲

	constexpr int kBattleStartStagingTime = 120; // バトル開始時の演出時間
	constexpr int kBattleEndStagingTime = 150;	 // バトル終了時の演出時間
}

SceneMain::SceneMain():
	m_currentEnemyNum(0),
	m_enemySpawnTime(0),
	m_battleStartStagingTime(0),
	m_battleEndStagingTime(0),
	m_isDispBattleStart(false),
	m_isBattleEndStaging(false),
	m_isPause(false),
	m_isLoading(true)
{
	// 非同期読み込み設定に変更
	SetUseASyncLoadFlag(true);

	m_modelHandle.resize(kModelNum);
	m_pEnemy.resize(kEnemyKindNum);

	LoadModelHandle(); // モデルを読み込む
}

SceneMain::~SceneMain()
{
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

	// メニューを開いたとき
	if (input.IsTriggered(InputId::kMenu))
	{
		m_isPause = true;
		return std::make_shared<SceneMenu>(shared_from_this(), m_pPlayer, m_pCamera);
	}

	// バトル開始演出
	UpdateBattleStartStaging();
	// バトル終了演出
	UpdateBattleEndStaging();

	// 敵が1体もいなくなった場合
	if (m_pEnemy.empty() && !m_isBattleEndStaging)
	{
		// バトル終了状態にする
		m_pPlayer->SetIsBattle(false);

		// 敵を生成する
		CreateEnemy();
	}

	UpdateEnemy();
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

	// プレイヤーのバーUI表示
	m_pUiBar->DrawPlayerHpBar(*m_pPlayer, m_pPlayer->GetStatus().maxHp);
	m_pUiBar->DrawPlayerGaugeBar(*m_pPlayer, m_pPlayer->GetStatus().maxGauge);

	// バトル終了演出中は表示しない
	if (!m_isBattleEndStaging)
	{
		// ミニマップを表示
		m_pUi->DrawMiniMap(*m_pPlayer, m_pEnemy);
	}
	
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
	m_pEventData->Draw();
#endif
}

void SceneMain::LoadModelHandle()
{
	// プレイヤー
	m_modelHandle[CharacterBase::CharaType::kPlayer] = MV1LoadModel(kPlayerHandlePath);

	// 敵
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 2桁にそろえる
		char enemyId[3];
		sprintf_s(enemyId, "%02d", (i + 1));
		m_modelHandle[(i + 1)] = MV1LoadModel((kEnemyHandlePath + std::string(enemyId) + ".mv1").c_str());
	}
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

void SceneMain::UpdateBattleEndStaging()
{
	// 演出中でない場合は飛ばす
	if (!m_isBattleEndStaging) return;

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
		sound.PlayLoopBgm(SoundName::kBgm_battle);
	}
	else
	{
		sound.StopBgm(SoundName::kBgm_battle);
		sound.PlayLoopBgm(SoundName::kBgm_congestion);
	}
	
}

void SceneMain::CreateEnemy()
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
	}

	// erase-removeイディオムで特定の要素(nullptr)だけを削除する
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

	std::unordered_set<int> enemyKind(enemySpawnNum);  // 生成された敵の種類を保持する
	std::unordered_set<int> enemyName(enemySpawnNum);  // 決まった敵名を保持する

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 敵の種類が重複しないようにする
		int enemyIndex;
		do
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution urdIndex(1, kEnemyKindNum);
			enemyIndex = urdIndex(mt);
		} while (enemyKind.count(enemyIndex) > 0);	 // MEMO:countは要素が見つかったら1を、見つからない場合は0を返す

		enemyKind.insert(enemyIndex);

		// 敵名が重複しないようにする
		int enemyNameIndex;
		do
		{
			// 敵の名前をランダムで選ぶ
			std::random_device rd;
			std::mt19937 mt(rd());
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
		m_pEnemy[i]->Init();
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

		// 当たっている場合
		if (isCol)
		{
			// イベントIDに応じた処理を行う
			StartEvent(event.eventId);
		}
	}
}

void SceneMain::StartEvent(const std::string& eventId)
{
	// IDに応じて処理を変更する
	if (eventId == "bossBattle")
	{
		printfDx("ボスバトル開始\n");
	}
}
