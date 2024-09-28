#pragma once
#include <memory>
#include <string>

class Player;
class Input;

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

	PlayerStateBase(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	virtual void Update(Input input) = 0;
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
};