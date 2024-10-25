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
	constexpr int kModelNum = 2;	 // 読み込むモデルの数
	constexpr int kEnemyKindNum = 1; // 敵の種類
}

SceneMain::SceneMain()
{
	// TODO:非同期処理

	m_modelHandle.resize(kModelNum);
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
	m_pEnemy->Init();
	m_pPlayer->Init();
	m_pWeapon->Init();
	m_pCamera->Init();
}

std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	m_pPlayer->Update(input, *m_pCamera, *m_pStage, *m_pWeapon, m_pEnemy);
	m_pWeapon->Update(*m_pPlayer, *m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);

	if (m_pEnemy != nullptr)
	{
		// 敵死亡フラグがtrueの時、敵を消滅させる
		if (m_pEnemy->GetIsDead())
		{
			m_pEnemy = nullptr;
		}
		else
		{
			m_pEnemy->Update(*m_pStage, *m_pPlayer);
		}
	}
	// 敵を生成する
	else
	{
		SelectEnemy();
		m_pEnemy->Init();
	}

	return shared_from_this();
}

void SceneMain::Draw()
{
	m_pStage->Draw();
	m_pWeapon->Draw();
	if (m_pEnemy != nullptr) m_pEnemy->Draw();
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
	for (int i = 0; i < kEnemyKindNum; i++)
	{
		// 2桁にそろえる
		char enemyId[3];
		sprintf_s(enemyId, "%02d", 1);
		m_modelHandle[(i + 1)] = MV1LoadModel((kEnemyHandlePath + std::string(enemyId) + ".mv1").c_str());
	}
}

void SceneMain::SelectEnemy()
{
	// 敵をランダムで選ぶ
	int enemyNum = GetRand((kEnemyKindNum - 1)) + 1;
	// 2桁にそろえる
	char enemyId[3];
	sprintf_s(enemyId, "%02d", enemyNum);

	m_pEnemy = std::make_shared<EnemyBase>("enemy_" + std::string(enemyId), enemyNum, m_modelHandle[enemyNum]);
}
