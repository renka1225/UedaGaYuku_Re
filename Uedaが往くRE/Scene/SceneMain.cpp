#include "Game.h"
#include "Input.h"
#include "UiBase.h"
#include "Camera.h"
#include "LoadCsv.h"
#include "CharacterBase.h"
#include "Player.h"
#include "EnemyBase.h"
#include "ObjectBase.h"
#include "Weapon.h"
#include "Stage.h"
#include "SceneMain.h"
#include <unordered_set>

// 定数
namespace
{
	const char* kPlayerHandlePath = "data/model/chara/player.mv1";	// プレイヤーのモデルハンドルパス
	const char* kEnemyHandlePath = "data/model/chara/enemy_";		// 敵のモデルハンドルパス
	constexpr int kModelNum = 3;		// 読み込むモデルの数
	constexpr int kEnemyMaxNum = 2;		// 1度に出現する最大の敵数
	constexpr int kEnemyKindNum = 2;	// 敵の種類
	constexpr int kEnemyNamekind = 10;	// 敵名の種類
}

SceneMain::SceneMain()
{
	// TODO:非同期処理

	m_modelHandle.resize(kModelNum);
	m_pEnemy.resize(kEnemyKindNum);
	LoadModelHandle(); // モデルを読み込む

	m_pPlayer = std::make_shared<Player>(m_modelHandle[CharacterBase::CharaType::kPlayer]);
	m_pWeapon = std::make_shared<Weapon>();
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>(m_pPlayer);
	m_pUI = std::make_shared<UiBase>(m_pPlayer);

	SelectEnemy(); // 敵の種類を決める
}

void SceneMain::Init()
{
	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->Init();
	}
	m_pPlayer->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
}

std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	// 敵同士の当たり判定をチェックする
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 敵が1対もいない場合、敵を生成する
		if (m_pEnemy.empty())
		{
			if (input.IsTriggered(InputId::kDebugSpawn))
			{
				SelectEnemy();
				m_pEnemy[i]->Init();
			}
		}

		if (m_pEnemy[i] == nullptr) continue;

		// 敵死亡フラグがtrueの時、敵を消滅させる
		if (m_pEnemy[i]->GetIsDead())
		{
			m_pEnemy[i] = nullptr;
		}
		else
		{
			m_pEnemy[i]->Update(*m_pStage, *m_pPlayer);
		}

		for (int j = 0; j < m_pEnemy.size(); j++)
		{
			if((i == j) || (m_pEnemy[i] == nullptr) || (m_pEnemy[j] == nullptr)) continue;
			m_pEnemy[i]->CheckCharaCol(*m_pEnemy[j], m_pEnemy[j]->GetCol(j), j);
		}
	}

	m_pPlayer->Update(input, *m_pCamera, *m_pStage, *m_pWeapon, m_pEnemy);
	m_pWeapon->Update(*m_pPlayer, *m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);

	return shared_from_this();
}

void SceneMain::Draw()
{
	m_pStage->Draw();
	m_pWeapon->Draw();
	for (auto& enemy : m_pEnemy)
	{
		if (enemy == nullptr) continue;
		enemy->Draw(*m_pPlayer);
	}
	m_pPlayer->Draw();
	m_pUI->Draw();

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

void SceneMain::SelectEnemy()
{
	LoadCsv::GetInstance().LoadEnemyName(); // 敵名を読み込む

	// 出現する敵の数をランダムで決定する
	//int enemySpawnNum = GetRand(kEnemyMaxNum - 1) + 1;
	int enemySpawnNum = 2;
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
			enemyIndex = GetRand(kEnemyKindNum - 1) + 1; // 敵をランダムで選ぶ
		} while (enemyKind.count(enemyIndex) > 0);		 // MEMO:countは要素が見つかったら1を、見つからない場合は0を返す。
		enemyKind.insert(enemyIndex);

		// 敵名が重複しないようにする
		int enemyNameIndex;
		do
		{
			enemyNameIndex = GetRand(kEnemyNamekind - 1) + 1; // 敵の名前をランダムで選ぶ
		} while (enemyName.count(enemyNameIndex) > 0);
		enemyName.insert(enemyNameIndex);
		std::string enemyName = LoadCsv::GetInstance().GetEnemyName(enemyNameIndex);
		
		// 2桁にそろえる
		char enemyId[3];
		sprintf_s(enemyId, "%02d", enemyIndex);

		m_pEnemy[i] = std::make_shared<EnemyBase>(*m_pPlayer, "enemy_" + std::string(enemyId), enemyIndex, m_modelHandle[enemyIndex]);
		//m_pEnemy[i]->SetEnemyName("Enemy");
		m_pEnemy[i]->SetEnemyName(enemyName);
	}
}