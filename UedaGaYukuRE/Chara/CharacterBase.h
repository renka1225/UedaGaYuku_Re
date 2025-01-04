#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <string>
#include <vector>
#include <map>

class UiBar;
class Item;

/// <summary>
/// アニメーション名
/// </summary>
namespace AnimName
{
	const char* const kIdleStand = "idle_stand";
	const char* const kIdleFight = "idle_fight";
	const char* const kKick = "kick";
	const char* const kKickRound = "kick_round";
	const char* const kKickHeat = "kick_heat";
	const char* const kPunch1 = "punch1";
	const char* const kPunch2 = "punch2";
	const char* const kPunch3 = "punch3";
	const char* const kPunch = "punch";
	const char* const kSpecialAttack = "special";
	const char* const kRun = "run";
	const char* const kWalk = "walk";
	const char* const kWalkFight = "walk_fight";
	const char* const kAvoid = "avoid";
	const char* const kGuard = "guard";
	const char* const kDamage = "damage";
	const char* const kDown = "down";
	const char* const kGrabEnemy = "grab_enemy";
	const char* const kGrabOneHandWeapon = "grab_oneHandWeapon";
	const char* const kGrabTwoHandWeapon = "grab_twoHandWeapon";
}

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class CharacterBase : public ObjectBase
{
public:
	// キャラクターの種類
	enum CharaType
	{
		kPlayer,	 // プレイヤー
		kEnemy_01,	 // 敵1
		kEnemy_02,	 // 敵2
		kEnemy_03,	 // 敵3
		kEnemy_boss, // ボス
	};

	// キャラクターのステータス
	struct Status
	{
		float maxHp;				 // 最大HP
		float maxGauge;				 // 最大ゲージ量
		float walkSpeed;			 // 歩き速度
		float runSpeed;				 // 走り速度
		float avoidDist;			 // 回避距離
		float atkPowerPunch1;		 // 攻撃力_パンチ1コンボ目
		float atkPowerPunch2;		 // 攻撃力_パンチ2コンボ目
		float atkPowerPunch3;		 // 攻撃力_パンチ3コンボ目
		float atkPowerKick;			 // 攻撃力_キック
		float atkPowerOneHandWeapon; // 攻撃力_片手武器
		float atkPowerTwoHandWeapon; // 攻撃力_両手武器
	};

	// 当たり判定情報
	struct ColData
	{
		VECTOR bodyStartPos;		// 全身の当たり判定始点
		VECTOR bodyEndPos;			// 全身の当たり判定終点
		VECTOR bodyUpdateStartPos;	// 更新後の全身の当たり判定始点
		VECTOR bodyUpdateEndPos;	// 更新後の全身の当たり判定終点
		VECTOR leftShoulderPos;		// 左肩
		VECTOR leftForeArmPos;		// 左肘
		VECTOR leftHandPos;			// 左手首
		VECTOR rightShoulderPos;	// 右肩
		VECTOR rightForeArmPos;		// 右肘
		VECTOR rightHandPos;		// 右手首
		VECTOR leftUpLegPos;		// 左もも
		VECTOR leftLegPos;			// 左膝
		VECTOR leftFootPos;			// 左足首
		VECTOR leftEndPos;			// 左つま先
		VECTOR rightUpLegPos;		// 右もも
		VECTOR rightLegPos;			// 右膝
		VECTOR rightFootPos;		// 右足首
		VECTOR rightEndPos;			// 右足終点
		float bodyRadius;			// 全身の当たり判定の半径
		float armRadius;			// 腕の当たり判定の半径
		float legRadius;			// 脚の当たり判定の半径
	};

	// アニメーション情報
	struct AnimInfo
	{
		float loopFrame;	// 開始フレーム
		float playSpeed;	// アニメーションの速度
		int startupFrame;	// 発生フレーム数
		int activeFrame;	// 持続フレーム数
		int recoveryFrame;	// 硬直フレーム数
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// ダメージを受けた際の処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void OnDamage(float damage);

	/// <summary>
	/// キャラクター同士の当たり判定をチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="colData">当たり判定情報参照</param>
	/// <param name="charaType">相手側の種類</param>
	void CheckCharaCol(ObjectBase& obj, const CharacterBase::ColData& colData, int charaType);

	/// <summary>
	/// パンチ時の当たり判定をチェックする
	/// </summary>
	/// <param name="colData">当たり判定情報参照</param>
	/// <param name="charaType">相手側の種類</param>
	bool CheckHitPunchCol(const CharacterBase::ColData& colData, int charaType);

	/// <summary>
	/// キック時の当たり判定をチェックする
	/// </summary>
	/// <param name="colData">当たり判定情報参照</param>
	/// <param name="charaType">相手側の種類</param>
	bool CheckHitKickCol(const CharacterBase::ColData& colData, int charaType);

	/// <summary>
	/// アニメーションを変更
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	void ChangeAnim(std::string animName);

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void UpdateAnim();

	/// <summary>
	/// アニメーションをスローモーションで再生する
	/// </summary>
	void SlowAnim();

	/// <summary>
	/// アニメーションの再生速度を戻す
	/// </summary>
	void ResetAnim();

	/// <summary>
	/// 回避状態を更新する
	/// </summary>
	void UpdateAvoid();

	/// <summary>
	/// 攻撃状態をセットする
	/// </summary>
	/// <param name="isGrab">攻撃中かどうか</param>
	void SetIsAttack(bool isAttack) { m_isAttack = isAttack; }

	/// <summary>
	/// 無敵状態をセットする
	/// </summary>
	/// <param name="isInvincible">無敵中かどうか</param>
	void SetIsInvincible(bool isInvincible) { m_isInvincible = isInvincible; }

	/// <summary>
	/// ガード状態をセットする
	/// </summary>
	void SetIsGuard(bool isGuard) { m_isGuard = isGuard; }

	/// <summary>
	/// 死亡フラグをセットする
	/// </summary>
	void SetIsDead(bool isDead) { m_isDead = isDead; }

	/// <summary>
	/// ダメージを受けている状態かどうかセットする
	/// </summary>
	void SetIsOnDamage(bool isOnDamage) { m_isOnDamage = isOnDamage; }

	/// <summary>
	/// 武器の掴み可能、不可能をセットする
	/// </summary>
	void SetIsPossibleGrabWeapon(bool isGrab) { m_isPossibleGrabWeapon = isGrab; }

	/// <summary>
	/// 武器掴み状態をセットする
	/// </summary>
	/// <param name="isGrab">掴んだかどうか</param>
	void SetIsGrabWeapon(bool isGrab) { m_isNowGrabWeapon = isGrab; }

	/// <summary>
	/// 移動可能かどうかセットする
	/// </summary>
	void SetIsPossibleMove(bool isMove) { m_isPossibleMove = isMove; }

	/// <summary>
	/// アニメーションの総再生時間を取得する
	/// </summary>
	float GetAnimTotalTime(std::string animName);

	/// <summary>
	/// アニメーション番号を取得する
	/// </summary>
	int GetAnimIndex(std::string animName);

	/// <summary>
	/// キャラクターのステータス取得
	/// </summary>
	Status GetStatus() const { return m_status; }

	/// <summary>
	/// アニメーションの速度を取得
	/// </summary>
	float GetAnimSpeed() const { return m_animPlaySpeed; }

	/// <summary>
	/// キャラクターのHPを取得
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() const { return m_hp; }

	/// <summary>
	/// キャラクターの角度を取得
	/// </summary>
	float GetAngle() const { return m_angle; }

	/// <summary>
	/// キャラクターのゲージ量を取得
	/// </summary>
	/// <returns>ゲージ量</returns>
	float GetGauge() const { return m_gauge; }

	/// <summary>
	/// 回避のクールタイムを取得
	/// </summary>
	/// <returns>回避のクールタイム</returns>
	int GetAvoidCoolTime() const { return m_avoidCoolTime; }

	/// <summary>
	/// キャラクターの3Dモデルを取得
	/// </summary>
	/// <returns>3Dモデルハンドル</returns>
	int GetHandle() const { return m_modelHandle; }

	/// <summary>
	/// 現在のアニメーション名を取得
	/// </summary>
	/// <returns>現在のアニメーション名</returns>
	std::string GetCurrentAnim() const { return m_currenAnimName; }

	/// <summary>
	/// 当たり判定情報を取得
	/// </summary>
	/// <param name="charType">キャラクターの種類</param>
	/// <returns>当たり判定情報</returns>
	ColData GetCol(int charType) const { return m_colData[charType]; }
	
	/// <summary>
	/// 攻撃中かどうか取得する
	/// </summary>
	/// <returns>攻撃中かどうか</returns>
	bool GetIsAttack() const { return m_isAttack; }

	/// <summary>
	/// 無敵中かどうか取得する
	/// </summary>
	/// <returns>無敵中かどうか</returns>
	bool GetIsInvincible() const { return m_isInvincible; }

	/// <summary>
	/// ガード中かどうか取得する
	/// </summary>
	bool GetIsGuard() const { return m_isGuard; }

	/// <summary>
	/// ダメージを受けている状態かどうか取得する
	/// </summary>
	bool GetIsOnDamage() const { return m_isOnDamage; }

	/// <summary>
	/// 死亡フラグを取得する
	/// </summary>
	/// <returns>死亡しているかどうか</returns>
	bool GetIsDead() const { return m_isDead; }

	/// <summary>
	/// 武器を掴める状態かどうか取得する
	/// </summary>
	/// <returns>武器を掴めるかどうか</returns>
	bool GetIsPossibleGrabWeapon() const { return m_isPossibleGrabWeapon; }

	/// <summary>
	/// 武器掴み中かどうか取得する
	/// </summary>
	/// <returns>武器掴み状態</returns>
	bool GetIsGrabWeapon() const { return m_isNowGrabWeapon; }

	/// <summary>
	/// 移動可能かどうか取得する
	/// </summary>
	bool GetIsPossibleMove() const { return m_isPossibleMove; }

protected:
	/// <summary>
	/// キャラクターの角度を更新
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	/// <param name="charType">キャラクターの種類</param>
	void UpdateCol(int charType);

	/// <summary>
	/// 位置ログを更新する
	/// </summary>
	void UpdatePosLog();

	/// <summary>
	/// 回避の場合残像を表示する
	/// </summary>
	void DrawAfterImage();

	/// <summary>
	/// SEを更新する
	/// </summary>
	void UpdateSe();

protected:
	std::shared_ptr<UiBar> m_pUiBar; // バーUIのポインタ
	std::shared_ptr<Item> m_pItem;	 // アイテムのポインタ
	std::map<std::string, AnimInfo> m_animData;	// アニメーションのデータ
	std::vector<ColData> m_colData; // 当たり判定情報
	std::vector<VECTOR> m_posLog;	// 過去の位置を保存する
	std::string m_currenAnimName;	// 現在再生中のアニメーション名を保存する
	Status m_status;				// ステータス

	float m_angle;					// 向いている方向の角度
	float m_hp;						// HP
	float m_gauge;					// ゲージ量
	int m_avoidNum;					// 現在の回避の回数
	int m_avoidCoolTime;			// 回避のクールタイム

	bool m_isAttack;				// 攻撃中かどうか
	bool m_isInvincible;			// 無敵中かどうか
	bool m_isGuard;					// ガード中かどうか
	bool m_isOnDamage;				// ダメージを受けた状態かどうか
	bool m_isDead;					// 死亡したかどうか
	bool m_isPossibleGrabWeapon;	// 武器が掴める状態かどうか
	bool m_isNowGrabWeapon;			// 今武器を掴んでいるか
	bool m_isPossibleMove;			// 移動可能かどうか
	bool m_isAnimSlow;				// アニメーションをスローモーションで再生中かどうか

	int m_currentPlayAnim;			// 現在のアニメーション
	int m_prevPlayAnim;				// 前に再生していたアニメーション
	float m_animBlendRate;			// 現在と過去のアニメーションのブレンド率
	float m_currentAnimTime;		// 現在のアニメーション再生時間
	float m_prevAnimTime;			// 前のアニメーション再生時間
	float m_totalAnimTime;			// アニメーションの総再生時間
	float m_animPlaySpeed; 			// アニメーションの再生速度
	float m_animLoopStartTime;		// アニメーションがループが始まる時間
	float m_animLoopEndTime;		// アニメーションがループが終わる時間
	bool m_isLoopAnim;				// アニメーションをループさせるか
};

