#pragma once
#include "CharacterBase.h"
#include "EnemyStateBase.h"

class UiBar;
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
	/// <param name="pUi">UIのポインタ</param>
	/// <param name="player">プレイヤー参照</param>
	EnemyBase(std::shared_ptr<UiBar> pUi, Player& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase();

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
	virtual void Draw(Player& player);

	/// <summary>
	/// 敵の情報をセットする
	/// </summary>
	/// <param name="name">敵名</param>
	/// <param name="charaId">キャラクターID</param>
	/// <param name="index">キャラクターの番号</param>
	/// <param name="modelHandle">モデルハンドル</param>
	void SetEnemyInfo(std::string name, std::string charaId, int index, int modelHandle);

	/// <summary>
	/// 死亡フラグをセットする
	/// </summary>
	void SetIsDead(bool isDead) { m_isDead = isDead; }

	/// <summary>
	/// 敵からプレイヤーへの位置ベクトルを取得する
	/// </summary>
	/// <returns></returns>
	VECTOR GetEToPVec() const { return m_eToPVec; }

	/// <summary>
	/// 死亡フラグを取得する
	/// </summary>
	/// <returns>死亡しているかどうか</returns>
	bool GetIsDead() const { return m_isDead; }

	/// <summary>
	/// 敵の番号を取得する
	/// </summary>
	/// <returns>敵の番号</returns>
	int GetEnemyIndex() const { return m_enemyIndex; }

private:
	/// <summary>
	/// 敵のフレーム位置を取得する
	/// </summary>
	void GetFramePos();

protected:
	std::shared_ptr<EnemyStateBase> m_pState; // stateパターン
	std::shared_ptr<UiBar> m_pUiBar;		  // バーのUIを表示する
	std::string m_enemyName; // 敵名
	int m_enemyIndex;		 // 敵の番号
	VECTOR m_eToPVec;		 // 敵からプレイヤーまでの位置ベクトル
	bool m_isDead;			 // 死亡したかどうか
};

