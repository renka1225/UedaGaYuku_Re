#pragma once
#include "DxLib.h"
#include <memory>
#include <string>

class Input;
class Camera;
class Stage;
class Weapon;
class EnemyBase;
class Player;

/// <summary>
/// プレイヤーの状態を管理する基底クラス
/// </summary>
class PlayerStateBase : public std::enable_shared_from_this<PlayerStateBase>
{
public:
	// プレイヤーの状態
	enum class PlayerStateKind
	{
		kIdle,		// 待機
		kWalk,		// 歩き
		kRun,		// 走り
		kAttack,	// 攻撃
		kDamage,	// 攻撃を受ける
		kAvoid,		// 回避
		kGuard,		// ガード
		kGrab,		// 掴み
		kDeath,		// 死亡
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー参照</param>
	PlayerStateBase(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateBase() {};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy);
	
	/// <summary>
	/// 現在のStateを取得
	/// </summary>
	/// <returns>Stateの種類</returns>
	virtual PlayerStateKind GetKind() = 0;

#ifdef _DEBUG
	/// <summary>
	/// 現在の状態名を取得
	/// </summary>
	/// <returns>State名</returns>
	virtual std::string GetStateName() = 0;
#endif

protected:
	/// <summary>
	/// ステートを更新するかチェックする
	/// </summary>
	bool IsStateInterrupt();

	/// <summary>
	/// 待機状態に変更する
	/// </summary>
	void ChangeStateIdle();

	/// <summary>
	/// 攻撃状態に変更する
	/// </summary>
	/// <param name="input">入力状態</param>
	void ChangeStateAttack(const Input& input);

	/// <summary>
	/// 必殺技発動状態に変更する
	/// </summary>
	void ChangeStateSpecialAttack();

	/// <summary>
	/// ガード状態に変更する
	/// </summary>
	void ChangeStateGuard();

	/// <summary>
	/// 回避状態に変更する
	/// </summary>
	void ChangeStateAvoid();

	/// <summary>
	/// 掴み状態に変更する
	/// </summary>
	/// <param name="pWeapon">武器情報参照</param>
	void ChangeStateGrab(Weapon& pWeapon);
	
	/// <summary>
	/// ダメージ状態に変更する
	/// </summary>
	void ChangeStateDamage();

	/// <summary>
	/// 死亡状態に変更する
	// </summary>
	void ChangeStateDeath();

public:
	std::shared_ptr<PlayerStateBase> m_nextState;	// 次のStateを保存する

protected:
	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ

	VECTOR m_upMoveVec;		// 上ボタンを入力をしたときの移動方向ベクトル
	VECTOR m_leftMoveVec;	// 左ボタンを入力をしたときの移動方向ベクトル
	VECTOR m_moveVec;		// 移動ベクトル
	float m_animEndTime;	// アニメーションが終わるまでの時間
	bool m_isGuardEffect;	// ガードエフェクト再生中かどうか

	DINPUT_JOYSTATE m_analogInput; 	// アナログスティック情報取得
	int m_analogX;					// アナログスティックの左右の入力状態
	int m_analogY;					// アナログスティックの上下の入力状態
};