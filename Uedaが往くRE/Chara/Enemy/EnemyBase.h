#pragma once
#include "CharacterBase.h"
#include "EnemyStateBase.h"

class Player;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public CharacterBase, public std::enable_shared_from_this<EnemyBase>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="charaId">キャラクターID</param>
	/// <param name="number">キャラクターの番号</param>
	/// <param name="modelHandle">モデルハンドル</param>
	EnemyBase(Player& player, std::string charaId, int number, int modelHandle);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(Stage& stage, Player& player);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 死亡フラグをセットする
	/// </summary>
	void SetIsDead(bool isDead) { m_isDead = isDead; }

	/// <summary>
	/// 死亡フラグを取得する
	/// </summary>
	/// <returns>死亡しているかどうか</returns>
	bool GetIsDead() const { return m_isDead; }

	/// <summary>
	/// 敵の番号を取得する
	/// </summary>
	/// <returns>敵の番号</returns>
	int GetEnemyNumber() const { return m_enemyNumber; }

private:
	/// <summary>
	/// 敵のフレーム位置を取得する
	/// </summary>
	void GetFramePos();

protected:
	std::shared_ptr<EnemyStateBase> m_pState; // stateパターン
	int m_enemyNumber; // 敵の番号
	bool m_isDead;	   // 死亡したかどうか
};

