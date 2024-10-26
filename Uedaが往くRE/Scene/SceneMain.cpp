#include "Camera.h"
#include "UiBase.h"
#include "CharacterBase.h"
#include "Player.h"
#include "EnemyBase.h"
#include "ObjectBase.h"
#include "Weapon.h"
#include "Stage.h"
#include "SceneMain.h"

// 定数
namespace
{
	const char* kPlayerHandlePath = "data/model/chara/player.mv1";	// プレイヤーのモデルハンドルパス
	const char* kEnemyHandlePath = "data/model/chara/enemy_";		// 敵のモデルハンドルパス
	constexpr int kModelNum = 3;	 // 読み込むモデルの数
	constexpr int kEnemyMaxNum = 2;	 // 1度に出現する最大の敵数
	constexpr int kEnemyKindNum = 2; // 敵の種類
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
	for (auto& enemy : m_pEnemy)
	{
		// 敵が1対もいない場合、敵を生成する
		if (m_pEnemy.empty())
		{
			SelectEnemy();
			enemy->Init();
		}

		if (enemy == nullptr) continue;
		// 敵死亡フラグがtrueの時、敵を消滅させる
		if (enemy->GetIsDead())
		{
			enemy = nullptr;
		}
		else
		{
			enemy->Update(*m_pStage, *m_pPlayer);
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
		enemy->Draw();
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
	// 出現する敵の数をランダムで決定する
	int enemySpawnNum = GetRand(kEnemyMaxNum - 1) + 1;
	m_pEnemy.resize(enemySpawnNum);
	printfDx("%d", enemySpawnNum);

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		// 敵をランダムで選ぶ
		int enemyIndex = GetRand(kEnemyKindNum);
		// 2桁にそろえる
		char enemyId[3];
		sprintf_s(enemyId, "%02d", enemyIndex);

		m_pEnemy[i] = std::make_shared<EnemyBase>(*m_pPlayer, "enemy_" + std::string(enemyId), enemyIndex, m_modelHandle[enemyIndex]);
	}
}
