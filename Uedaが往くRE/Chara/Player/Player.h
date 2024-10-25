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
	virtual void Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::shared_ptr<EnemyBase> pEnemy);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーの角度を更新
	/// </summary>
	void UpdateAngle();

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
	/// 現在の所持金額を取得する
	/// </summary>
	/// <returns></returns>
	int GetMoney() const { return m_money; }

private:
	/// <summary>
	/// プレイヤーのフレーム位置を取得する
	/// </summary>
	void GetFramePos();

private:
	std::shared_ptr<PlayerStateBase> m_pState;	// stateパターン
	
	int m_money;		// 所持金額
	int m_beforeMoney;	// 増減前の金額
	int m_addMoney;		// 追加する金額
};

