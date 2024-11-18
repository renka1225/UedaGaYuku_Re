#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class UiBase;
class EnemyBase;
class Player;
class Weapon;
class Camera;
class Stage;

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

	/// <summary>
	/// モデルハンドルを読み込む
	/// </summary>
	void LoadModelHandle();

private:
	/// <summary>
	/// バトル開始演出の更新を行う
	/// </summary>
	void UpdateBattleStartStaging();

	/// <summary>
	/// 敵を生成する
	/// </summary>
	void CreateEnemy();

	/// <summary>
	/// 敵を更新する
	/// </summary>
	void UpdateEnemy();

	/// <summary>
	/// 敵をランダムで決定する
	/// </summary>
	void SelectEnemy();

private:
	std::vector<std::shared_ptr<EnemyBase>> m_pEnemy; // 敵のポインタ
	std::shared_ptr<Player> m_pPlayer;	 // プレイヤーのポインタ
	std::shared_ptr<Weapon> m_pWeapon;	 // 武器のポインタ
	std::shared_ptr<Camera> m_pCamera;	 // カメラのポインタ
	std::shared_ptr<Stage> m_pStage;	 // ステージのポインタ
	std::shared_ptr<UiBar> m_pUiBar;	 // キャラクターバーUIのポインタ
	std::vector<int> m_modelHandle;		 // モデルのハンドル

	int m_enemySpawnTime;			// 敵がスポーンするまでの時間
	int m_battleStartStagingTime;	// バトル開始演出の時間

	bool m_isDispBattleStart;	// バトル開始演出が再生中かどうか
	bool m_isPause;				// ポーズ状態かどうか(true:ポーズ状態)
};