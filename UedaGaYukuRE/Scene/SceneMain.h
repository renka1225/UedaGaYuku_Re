#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class UiBase;
class UiMain;
class UiBar;
class EnemyBase;
class Player;
class Npc;
class Weapon;
class Camera;
class Stage;
class EventData;

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
	virtual ~SceneMain();

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
	///	ローディング中の処理
	/// </summary>
	void Loading();

	/// <summary>
	/// ローディング後の初期化
	/// </summary>
	void InitAfterLoading();

	/// <summary>
	/// 演出の更新を行う
	/// </summary>
	void UpdateStaging();

	/// <summary>
	/// バトル開始演出の更新を行う
	/// </summary>
	void UpdateBattleStartStaging();

	/// <summary>
	/// バトル終了演出の更新を行う
	/// </summary>
	std::shared_ptr<SceneBase> UpdateBattleEndStaging();

	/// <summary>
	/// エンディング演出の更新を行う
	/// </summary>
	/// <returns>遷移するシーン</returns>
	std::shared_ptr<SceneBase> UpdateEndingStaging();

	/// <summary>
	/// サウンド再生の処理を行う
	/// </summary>
	void UpdateSound();

	/// <summary>
	/// 敵を生成する
	/// </summary>
	void CreateEnemy();

	/// <summary>
	/// 敵を更新する
	/// </summary>
	void UpdateEnemy();

	/// <summary>
	/// ボス敵の更新を行う
	/// </summary>
	void UpdateBossEnemy();

	/// <summary>
	/// 敵をランダムで決定する
	/// </summary>
	void SelectEnemy();

	/// <summary>
	/// 敵を消滅させるかどうか判定する
	/// </summary>
	/// <param name="index">敵のインデックス</param>
	/// <returns>消滅させるかどうか</returns>
	bool IsExtinction(int index);

	/// <summary>
	/// イベントトリガーをチェックする
	/// </summary>
	/// <param name="input">入力状態</param>
	void CheckEventTrigger(const Input& input);

	/// <summary>
	/// 呼び出されたイベントを開始する
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="eventId">開始するイベントID</param>
	void StartEvent(const std::string& eventId, const Input& input);

	/// <summary>
	/// 会話情報を更新する
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateTalk(const Input& input);

	/// <summary>
	/// シャドウマップをセットする
	/// </summary>
	void SetShadowMap();

	/// <summary>
	/// シャドウマップの描画情報をセットする
	/// </summary>
	void DrawSetUpShadow();

private:
	std::vector<std::shared_ptr<EnemyBase>> m_pEnemy; // 敵のポインタ
	std::shared_ptr<Player> m_pPlayer;		// プレイヤーのポインタ
	std::shared_ptr<Npc> m_pNpc;			// NPCのポインタ
	std::shared_ptr<Weapon> m_pWeapon;		// 武器のポインタ
	std::shared_ptr<Camera> m_pCamera;		// カメラのポインタ
	std::shared_ptr<Stage> m_pStage;		// ステージのポインタ
	std::shared_ptr<UiMain> m_pUiMain;		// メイン用UIのポインタ
	std::shared_ptr<UiBar> m_pUiBar;		// キャラクターバーUIのポインタ
	std::shared_ptr<EventData> m_pEventData;// イベントデータのポインタ
	std::vector<int> m_modelHandle;			// モデルのハンドル

	std::string m_nowTalkId;	// 現在の会話ID

	int m_currentEnemyNum;			// 現在の敵数
	int m_playTime;					// 現在のプレイ時間
	int m_enemySpawnTime;			// 敵がスポーンするまでの時間
	int m_battleStartStagingTime;	// バトル開始演出の時間
	int m_battleEndStagingTime;		// バトル終了演出の時間
	int m_talkDispTime;				// 会話を表示させる最低限の時間
	int m_endingTime;				// エンディングの時間

	bool m_isDispBattleStart;	// バトル開始演出中かどうか(true:演出中)
	bool m_isBattleEndStaging;	// バトル終了演出中か(true:演出中)
	bool m_isEnding;			// エンディング演出中か(true:演出中)
	bool m_isPause;				// ポーズ状態かどうか(true:ポーズ状態)
	bool m_isLoading;			// ロード中か(true:ロード中)
	bool m_isTutorial;			// チュートリアル中か(true:チュートリアル中)
	bool m_isLastBattle;		// ボス戦かどうか

	int m_shadowMap; // シャドウマップ
};