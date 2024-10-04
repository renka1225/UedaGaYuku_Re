#pragma once
#include "SceneBase.h"
#include <memory>

class EnemyBase;
class Player;
class Camera;
class Stage;

/// <summary>
/// メインシーン
/// </summary>
class SceneMain :public SceneBase
{
public:
	SceneMain();
	virtual ~SceneMain();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	std::shared_ptr<EnemyBase> m_pEnemy; // 敵のポインタ
	std::shared_ptr<Player> m_pPlayer;	 // プレイヤーのポインタ
	std::shared_ptr<Camera> m_pCamera;	 // カメラのポインタ
	std::shared_ptr<Stage> m_pStage;	 // ステージのポインタ
};

