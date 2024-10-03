#pragma once
#include "DxLib.h"
#include <memory>
#include <string>

class Input;
class Camera;
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
		kIdle,	// 待機
		kWalk,	// 歩き
		kRun	// 走り
	};

	PlayerStateBase(std::shared_ptr<Player> pPlayer);
	virtual void Update(const Input& input, const Camera& camera) = 0;
	// 現在の状態を取得
	virtual PlayerStateKind GetKind() = 0;

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() = 0;
#endif

public:
	std::shared_ptr<PlayerStateBase> m_nextState;	// 次のStateを保存する

protected:
	std::shared_ptr<Player> m_pPlayer;				// プレイヤーのポインタ

	DINPUT_JOYSTATE m_analogInput; 	// アナログスティック情報取得
	int m_analogX;	// アナログスティックの左右の入力状態
	int m_analogY;	// アナログスティックの上下の入力状態
};