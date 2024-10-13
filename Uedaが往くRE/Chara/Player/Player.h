#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"

class Input;
class Camera;
class Stage;
class EnemyBase;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

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
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, EnemyBase& enemy);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーの角度を更新
	/// </summary>
	void UpdateAngle();

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// stateパターン
	int m_money;	// 所持金額
};

