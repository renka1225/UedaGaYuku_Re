#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"

class DebugDraw;
class Input;
class Camera;
class Stage;
class Weapon;
class EnemyBase;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	// 現在の強化段階
	struct EnhanceStep
	{
		int nowHpUpStep = 0;	// 最大HP強化段階
		int nowGaugeUpStep = 0;	// ゲージ量強化段階
		int nowAtkUpStep = 0;	// 攻撃力強化段階
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	Player(int modelHandle);

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
	/// 所持金を更新する
	/// </summary>
	/// <param name="dropMoney">敵がドロップした金額</param>
	void UpdateMoney();

	/// <summary>
	/// 所持金を増やす
	/// </summary>
	/// <param name="dropMoney">敵がドロップした金額</param>
	/// <returns>敵がドロップした金額</returns>
	void AddMoney(int dropMoney);

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
	/// 攻撃力アップ処理
	/// </summary>
	/// <param name="atkUpRate">攻撃力アップの割合</param>
	/// <param name="effectTime">効果時間</param>
	void AtkUp(float atkUpRate, int effectTime);

	/// <summary>
	/// プレイヤーのHPを強化する
	/// </summary>
	/// <param name="upAmount">上昇倍率</param>
	void EnhanceHpUp(float upAmount);

	/// <summary>
	/// プレイヤーのゲージ量を強化する
	/// </summary>
	/// <param name="skillID">スキルID</param>
	void EnhanceGauge(float upAmount);

	/// <summary>
	/// プレイヤーの攻撃力を強化する
	/// </summary>
	/// <param name="skillID">スキルID</param>
	void EnhanceAtkUp(float upAmount);

	/// <summary>
	/// 現在の所持金額を取得する
	/// </summary>
	/// <returns></returns>
	int GetMoney() const { return m_money; }

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

private:
	/// <summary>
	/// アイテムの効果を削除する
	/// </summary>
	void DeleteItemEffect();

	/// <summary>
	/// プレイヤーのフレーム位置を取得する
	/// </summary>
	void GetFramePos();

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// stateパターン
	std::vector<int> m_possessItem;	// プレイヤーが所持しているアイテム情報を保存しておく
	std::vector<VECTOR> m_pToEVec;	// プレイヤーから敵への位置ベクトル
	Status m_saveStatus;			// ステータスを一時保存する
	EnhanceStep m_enhanceStep;		// 現在の強化段階

	int m_money;			// 所持金額
	int m_beforeMoney;		// 増減前の金額
	int m_addMoney;			// 追加する金額
	int m_itemEffectTime;	// アイテムの効果時間
};

