#include "Camera.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Stage.h"
#include "SceneMain.h"

// 定数
namespace
{
	constexpr int kEnemyKindNum = 5;
}

SceneMain::SceneMain()
{
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>();

	SelectEnemy();
}

void SceneMain::Init()
{
	m_pEnemy->Init();
	m_pPlayer->Init();
	m_pCamera->Init();
}

std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	m_pEnemy->Update(*m_pStage);
	m_pPlayer->Update(input, *m_pCamera, *m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);

	return shared_from_this();
}

void SceneMain::Draw()
{
	m_pStage->Draw();
	m_pEnemy->Draw();
	m_pPlayer->Draw();

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
#endif
}

void SceneMain::SelectEnemy()
{
	// TODO:敵をランダムで選ぶ
	int enemyNum = GetRand((kEnemyKindNum - 1)) + 1;
	char enemyId[3]; // 2桁 + 終端文字
	sprintf_s(enemyId, "%02d", enemyNum);

	m_pEnemy = std::make_shared<EnemyBase>("enemy_" + std::string(enemyId));
}
