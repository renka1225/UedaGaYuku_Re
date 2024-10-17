#pragma once
#include "SceneBase.h"
#include <memory>

class EnemyBase;
class Player;
class Camera;
class Stage;
class UiBase;

/// <summary>
/// メインシーン
/// </summary>
class SceneMain :public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneMain();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneMain() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

private:
	/// <summary>
	/// 敵をランダムで決定する
	/// </summary>
	void SelectEnemy();

private:
	std::shared_ptr<EnemyBase> m_pEnemy; // 敵のポインタ
	std::shared_ptr<Player> m_pPlayer;	 // プレイヤーのポインタ
	std::shared_ptr<Camera> m_pCamera;	 // カメラのポインタ
	std::shared_ptr<Stage> m_pStage;	 // ステージのポインタ
	std::shared_ptr<UiBase> m_pUI;		 // UI表示のポインタ
};

