#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
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
	constexpr int kEnemySpawnMinTIme = 180;		// 敵がスポーンするまでの最小時間
	constexpr int kEnemySpawnMaxTIme = 3000;	// 敵がスポーンするまでの最大時間

	constexpr int kBattleStartStagingTime = 40;	// バトル開始時の演出時間
}

SceneMain::SceneMain():
	m_enemySpawnTime(0),
	m_battleStartStagingTime(0),
	m_isDispBattleStart(false),
	m_isPause(false)
{
	// TODO:非同期処理

	m_modelHandle.resize(kModelNum);
	m_pEnemy.resize(kEnemyKindNum);
	LoadModelHandle(); // モデルを読み込む

	m_pUiBar = std::make_shared<UiBar>();
	m_pPlayer = std::make_shared<Player>(m_pUiBar, m_modelHandle[CharacterBase::CharaType::kPlayer]);
	m_pWeapon = std::make_shared<Weapon>(m_pPlayer);
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>(m_pPlayer);

	SelectEnemy(); // 敵の種類を決める
}

void SceneMain::Init()
{
	if (m_isPause) return;

	m_pPlayer->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
	m_pUiBar->Init();
	m_pItem->Init();
	m_isPause = false;
}

std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	// メニューを開いたとき
	if (input.IsTriggered(InputId::kMenu))
	{
		m_isPause = true;
		return std::make_shared<SceneMenu>(shared_from_this(), m_pPlayer, m_pCamera);
	}

	// バトル開始演出の時間を更新する
	UpdateBattleStartStaging();

	// 敵が1体もいなくなった場合
	if (m_pEnemy.empty())
	{
		// プレイヤーを通常状態にする
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

	EffectManager::GetInstance().Update();

	return shared_from_this();
}

void SceneMain::Draw()
{
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

	// バトル中UI表示
	m_pUi->DrawBattleUi(*m_pPlayer);

	// プレイヤーのバーUI表示
	m_pUiBar->DrawPlayerHpBar(*m_pPlayer, m_pPlayer->GetStatus().maxHp);
	m_pUiBar->DrawPlayerGaugeBar(*m_pPlayer, m_pPlayer->GetStatus().maxGauge);

	// ミニマップを表示
	m_pUi->DrawMiniMap(*m_pPlayer);
	

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
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

			// バトル演出中移動できないようにする
			for (int i = 0; i < m_pEnemy.size(); i++)
			{
				m_pEnemy[i]->SetIsPossibleMove(false);
			}
			m_pPlayer->SetIsPossibleMove(false);
		}
		else
		{
			// 移動できるようにする
			for (int i = 0; i < m_pEnemy.size(); i++)
			{
				m_pEnemy[i]->SetIsPossibleMove(true);
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

void SceneMain::CreateEnemy()
{
	// スポーンするまでの時間をランダムで決める
	const int spawnTime = GetRand(kEnemySpawnMaxTIme) + kEnemySpawnMinTIme;
	m_enemySpawnTime++;

	if (m_enemySpawnTime >= spawnTime)
	{
		m_enemySpawnTime = 0;
		SelectEnemy();
	}
}

void SceneMain::UpdateEnemy()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		if (m_pEnemy[i] == nullptr) continue;

		// 特定の状態の場合、敵を消滅させる
		bool isExtinction = m_pEnemy[i]->GetIsDead() || (m_pEnemy[i]->GetPos().y <= 0.0f);
		if (isExtinction)
		{
			m_pEnemy[i] = nullptr;
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