#include "Camera.h"
#include "UiBase.h"
#include "Player.h"
#include "EnemyBase.h"
#include "ObjectBase.h"
#include "Weapon.h"
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
	m_pPlayer->Update(input, *m_pCamera, *m_pStage, m_pEnemy);
	m_pWeapon->Update(*m_pPlayer);
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
	m_pPlayer->Draw();
	m_pUI->Draw();

	if (m_pEnemy != nullptr) m_pEnemy->Draw();

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
#endif
}

void SceneMain::SelectEnemy()
{
	// TODO:モデル呼び出す時一瞬固まるのでどうにかする
#ifdef false
	// 敵をランダムで選ぶ
	int enemyNum = GetRand((kEnemyKindNum - 1)) + 1;
	char enemyId[3]; // 2桁にそろえる
	sprintf_s(enemyId, "%02d", enemyNum);

	m_pEnemy = std::make_shared<EnemyBase>("enemy_" + std::string(enemyId));
#else false
	m_pEnemy = std::make_shared<EnemyBase>("enemy_01");
#endif 
}
