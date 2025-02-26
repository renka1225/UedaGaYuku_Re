#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"

class DebugDraw;
class Input;
class UiBar;
class UiMain;
class Camera;
class Stage;
class Weapon;
class EnemyBase;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public CharacterBase, public std::enable_shared_from_this<Player>
{
private:
	// ボタンの入力履歴を保存する
	struct CommandInput
	{
		std::string button;	// 入力されたコマンド名
		int frameCount;		// 入力されたフレーム数
	};

public:
	// チュートリアル数
	enum TutorialNum
	{
		kTuto_0,
		kTuto_1,
		kTuto_2,
		kTuto_3,
		kTuto_4,
		kTuto_5,
		kTutoNum
	};

	// チュートリアル情報
	struct TutorialInfo
	{
		int tutorialChangeTime = 0;	// チュートリアルの切り替え時間
		int talkNum = 0;			// 現在のチュートリアル会話数
		int currentNum = 0;			// 現在のチュートリアル数
		int currentKnowledge = 0;	// 現在の心得表示数
		int currentPunch = 0;		// 現在のパンチ回数
		int currentKick = 0;		// 現在のキック回数
		int currentAvoid = 0;		// 現在回避回数
		int currentGuard = 0;		// 現在のガード回数
		int currentGrab = 0;		// 現在の掴み回数
		int currentWeaponAtk = 0;	// 現在の武器攻撃回数
		int currentHeat = 0;		// 現在のヒートアクション回数
		bool isNowKnowledge = true;	// 心得を表示中か
		bool isTalk = false;		// 会話中かどうか
		bool isNowChange = false;	// チュートリアル切り替え中か
		bool isMove = false;		// 移動したか
		bool isDush = false;		// ダッシュしたか
		bool isCameraMove = false;	// カメラ移動したか
		bool isPunch = false;		// パンチしたか
		bool isKick = false;		// キックしたか
		bool isAvoid = false;		// 回避したか
		bool isGuard = false;		// ガードしたか
		bool isGrab = false;		// 武器を掴んだか
		bool isWeaponAtk = false;	// 武器で攻撃したか
		bool isHeat = false;		// ヒートアクションを行ったか
		bool isEndTutorial = false;	// チュートリアルが終了したか
	};

	// 現在の強化段階
	struct EnhanceStep
	{
		int nowHpUpStep = 0;	// 最大HP強化段階
		int nowGaugeUpStep = 0;	// ゲージ量強化段階
		int nowAtkUpStep = 0;	// 攻撃力強化段階
	};

	// 特殊敵の撃破状態
	struct DestroySpecialEnemy
	{
		bool isBob = false;	 // Bob撃破状態
		bool isSato = false; // Sato撃破状態
		bool isAbe = false;	 // Abe撃破状態
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pUi">UIバーのポインタ</param>
	/// <param name="pUiMain">メインUIのポインタ</param>
	/// <param name="handle">モデルハンドル</param>
	Player(std::shared_ptr<UiBar> pUi, std::shared_ptr<UiMain> pUiMain, int modelHandle);

	/// <summary>
	/// デスクトップ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="stage">ステージ参照</param>
	/// <param name="pWeapon">武器情報参照</param>
	/// <param name="pEnemy">敵情報のポインタ</param>
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーがダメージを受けた際の処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void OnDamage(float damage);

	/// <summary>
	/// 攻撃時、プレイヤーの位置を調整する
	/// </summary>
	void AdjPosAttack();

	/// <summary>
	/// 一番近くの敵の方に向く処理
	/// </summary>
	void UpdateAngleNearEnemy();

	/// <summary>
	/// バトル終了時状態を更新する
	/// </summary>
	void UpdateBattleEnd();

	/// <summary>
	/// プレイヤーから一番近くにいる敵を探す
	/// </summary>
	/// <param name="range">調べる範囲</param>
	/// <returns>一番近くにいる敵のインデックス</returns>
	int FindNearEnemy(float range);

	/// <summary>
	/// プレイヤーが武器に近づいているか判断する
	/// </summary>
	/// <param name="weaponPos"></param>
	/// <returns></returns>
	bool IsNearWeapon(const VECTOR& weaponPos);

	/// <summary>
	/// 倒した敵数を増やす
	/// </summary>
	void AddDeadEnemyNum() { m_deadEnemyNum++; }

	/// <summary>
	/// 所持金を更新する
	/// </summary>
	void UpdateMoney();

	/// <summary>
	/// 所持金の増減を行う
	/// </summary>
	/// <param name="changeMoney">増減させる金額</param>
	void AddDecreaseMoney(int changeMoney);

	/// <summary>
	/// 取得アイテムを追加する
	/// </summary>
	/// <param name="itemType">取得したアイテムの種類</param>
	void AddItem(int itemType);

	/// <summary>
	/// 使用したアイテムを削除する
	/// </summary>
	/// <param name="selectNum">選択したアイテム番号/param>
	void DeleteItem(int selectNum);

	/// <summary>
	/// HP回復処理
	/// </summary>
	/// <param name="recovery">回復割合</param>
	void RecoveryHp(float recoveryRate);

	/// <summary>
	/// ゲージ回復処理
	/// </summary>
	/// <param name="recovery">回復割合</param>
	void RecoveryGauge(float recoveryRate);

	/// <summary>
	/// HP回復+ゲージ回復処理
	/// </summary>
	/// <param name="hpRecoveryRate">HP回復割合</param>
	/// <param name="gaugeRecoveryRate">ゲージ回復割合</param>
	void RecoveryHpGauge(float hpRecoveryRate, float gaugeRecoveryRate);

	/// <summary>
	/// 必殺技ゲージの更新を行う
	/// </summary>
	/// <param name="changeAmount">変化量</param>
	void UpdateGauge(float changeAmount);

	/// <summary>
	/// 攻撃力アップ処理
	/// </summary>
	/// <param name="atkUpRate">攻撃力アップの割合</param>
	/// <param name="effectTime">効果時間</param>
	void AtkUp(float atkUpRate, int effectTime);

	/// <summary>
	/// プレイヤーのHPを強化する
	/// </summary>
	/// <param name="upAmount">上昇倍率</param>
	/// <param name="money">使用金額</param>
	void EnhanceHpUp(float upAmount, int money);

	/// <summary>
	/// プレイヤーのゲージ量を強化する
	/// </summary>
	/// <param name="skillID">スキルID</param>
	/// <param name="money">使用金額</param>
	void EnhanceGauge(float upAmount, int money);

	/// <summary>
	/// プレイヤーの攻撃力を強化する
	/// </summary>
	/// <param name="skillID">スキルID</param>
	/// <param name="money">使用金額</param>
	void EnhanceAtkUp(float upAmount, int money);

	/// <summary>
	/// チュートリアルの情報を更新する
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="pEnemy">敵参照</param>
	void UpdateTutorial(const Input& input, EnemyBase& pEnemy);

	/// <summary>
	/// チュートリアルを切り替える
	/// </summary>
	/// <param name="input">入力情報</param>
	void ChangeTutorial(const Input& input);

	/// <summary>
	/// 特殊バトルの開始状態をセットする
	/// </summary>
	void SetSpecialBattleInit();

	/// <summary>
	/// バトル状態をセットする
	/// </summary>
	/// <param name="isBattle">バトル中かどうか</param>
	void SetIsBattle(bool isBattle) { m_isBattle = isBattle; }

	/// <summary>
	/// NPCとの会話状態をセットする
	/// </summary>
	/// <param name="isTalk">会話できるかどうか</param>
	void SetIsTalk(bool isTalk) { m_isTalk = isTalk; }

	/// <summary>
	/// NPCと会話中かどうかセットする
	/// </summary>
	/// <param name="isNowTalk">会話中かどうか</param>
	void SetIsNowTalk(bool isNowTalk) { m_isNowTalk = isNowTalk; }

	/// <summary>
	/// 現在のチュートリアル情報を取得する
	/// </summary>
	/// <returns></returns>
	TutorialInfo GetTutoInfo() const { return m_tutorial; }

	/// <summary>
	/// 現在の所持金額を取得する
	/// </summary>
	/// <returns></returns>
	int GetMoney() const { return m_money; }

	/// <summary>
	/// アイテムを追加で入手できるかどうか取得する
	/// </summary>
	/// <returns>アイテムを取得できるかどうか</returns>
	bool GetIsAddItem() const { return m_isAddItem; }

	/// <summary>
	/// 現在所持しているアイテムを取得する
	/// </summary>
	/// <returns></returns>
	std::vector<int> GetPossessItem() const { return m_possessItem; }

	/// <summary>
	/// プレイヤーの強化状態を取得する
	/// </summary>
	/// <returns>強化段階の情報</returns>
	EnhanceStep GetEnhanceStep() const { return m_enhanceStep; }

	/// <summary>
	/// 特殊敵の撃破状態を取得する
	/// </summary>
	/// <returns>特殊敵の撃破状態</returns>
	DestroySpecialEnemy GetDestroySpecialEnemy() const { return m_destroySpecialEnemy; }

	/// <summary>
	/// 倒した敵数を取得する
	/// </summary>
	/// <returns>倒した敵数</returns>
	int GetDeadEnemyNum() const { return m_deadEnemyNum; }

	/// <summary>
	/// バトル中かどうか取得する
	/// </summary>
	/// <returns>バトル中かどうか</returns>
	bool GetIsBattle() const { return m_isBattle; }

	/// <summary>
	/// 必殺技を出せるかどうか取得する
	/// </summary>
	/// <returns>必殺技を出せるかどうか</returns>
	bool GetIsSpecial() const { return m_isSpecial; }

	/// <summary>
	/// 会話できるかどうか取得する
	/// </summary>
	/// <returns>会話できるかどうか</returns>
	bool GetIsTalk() const { return m_isTalk; }

	/// <summary>
	/// 現在会話中かどうか取得する
	/// </summary>
	/// <returns>会話中かどうか</returns>
	bool GetIsNowTalk() const { return m_isNowTalk; }

	/// <summary>
	/// 入力状態を取得する
	/// </summary>
	/// <returns>入力履歴</returns>
	std::vector<CommandInput> GetInputLog() const { return m_inputLog; }

	/// <summary>
	/// 入力コマンドをチェックする
	/// </summary>
	/// <param name="command">入力されたボタン</param>
	/// <param name="inputLog">ボタンログ</param>
	/// <returns>入力されたか</returns>
	bool CheckCommand(const std::vector<std::string>& command, const std::vector<CommandInput>& inputLog);

private:
	/// <summary>
	/// プレイヤーの向きを更新する
	/// </summary>
	virtual void UpdateAngle();

	/// <summary>
	/// セーブデータの情報を適用する
	/// </summary>
	void ApplySaveData();

	/// <summary>
	/// 敵の情報を更新する
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	void UpdateEnemyInfo(std::vector<std::shared_ptr<EnemyBase>> pEnemy);

	/// <summary>
	/// 武器との当たり判定情報を更新する
	/// </summary>
	/// <param name="weapon">武器情報参照</param>
	void UpdateWeaponColInfo(Weapon& weapon);

	/// <summary>
	/// バトル状態を更新する
	/// </summary>
	/// <param name="enemyIndex">敵の番号</param>
	void UpdateBattle(int enemyIndex);

	/// <summary>
	/// アイテム情報を更新する
	/// </summary>
	void UpdateItemInfo();

	/// <summary>
	/// アイテムの効果を削除する
	/// </summary>
	void DeleteItemEffect();

	/// <summary>
	/// プレイヤーのフレーム位置を取得する
	/// </summary>
	void GetFramePos();

	/// <summary>
	/// 入力コマンドを更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="currentFrame">現在のフレーム</param>
	void UpdateInputLog(const Input& input, int currentFrame);

	/// <summary>
	/// チュートリアル0
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateTuto0(const Input& input);

	/// <summary>
	/// チュートリアル1
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateTuto1(const Input& input);

	/// <summary>
	/// チュートリアル2
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateTuto2(const Input& input);

	/// <summary>
	/// チュートリアル3
	/// </summary>
	/// <param name="input">入力情報</param> 
	void UpdateTuto3(const Input& input);

	/// <summary>
	/// チュートリアル4
	/// </summary>
	/// <param name="input">入力情報</param> 
	/// <param name="pEnemy">敵参照</param> 
	void UpdateTuto4(const Input& input, EnemyBase& pEnemy);

	/// <summary>
	/// チュートリアル5
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateTuto5(const Input& input);

	/// <summary>
	/// チュートリアルを終了する
	/// </summary>
	void EndTutorial();

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// stateパターン
	std::shared_ptr<UiMain> m_pUiMain;		// メインシーンで使用するUIポインタ
	std::vector<VECTOR> m_pToEVec;			// プレイヤーから敵への位置ベクトル
	std::vector<int> m_possessItem;			// プレイヤーが所持しているアイテム情報を保存しておく
	std::vector<CommandInput> m_inputLog;	// 入力情報を一時保存する
	Status m_saveStatus;					// ステータスを一時保存する
	TutorialInfo m_tutorial;				// チュートリアルの情報
	EnhanceStep m_enhanceStep;				// 現在の強化段階
	DestroySpecialEnemy m_destroySpecialEnemy;	// 特殊敵の撃破状態

	int m_deadEnemyNum;		 // 倒した敵数
	int m_money;			 // 所持金額
	int m_beforeMoney;		 // 増減前の金額
	int m_addMoney;			 // 追加する金額
	int m_currentInputFrame; // 現在の入力フレーム数
	int m_itemEffectTime;	 // アイテムの効果時間
	bool m_isAddItem;		 // アイテムを追加で取得できるか(true:取得可能)
	bool m_isSpecial;		 // 必殺技が出せるかどうか
	bool m_isTalk;			 // NPCと会話できるかどうか
	bool m_isNowTalk;		 // 今会話中かどうか

	int m_battleStartCount;	 // バトルが開始するまでの時間
	bool m_isBattle;		 // バトル状態かどうか(true:バトル中)
};