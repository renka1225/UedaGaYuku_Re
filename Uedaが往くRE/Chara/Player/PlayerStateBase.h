#pragma once
#include "DxLib.h"
#include <memory>
#include <string>

class Input;
class Camera;
class Stage;
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
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, std::shared_ptr<EnemyBase> pEnemy);
	
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

public:
	std::shared_ptr<PlayerStateBase> m_nextState;	// 次のStateを保存する

protected:
	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ

	DINPUT_JOYSTATE m_analogInput; 		// アナログスティック情報取得
	int m_analogX;						// アナログスティックの左右の入力状態
	int m_analogY;						// アナログスティックの上下の入力状態
};