#pragma once
#include "EnemyStateBase.h"
#include <memory>

class EnemyBase;

/// <summary>
/// 敵AIを管理する
/// </summary>
class EnemyAI
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pEnemy">敵ポインタ</param>
	EnemyAI(std::shared_ptr<EnemyBase> pEnemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyAI() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() {}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 次の行動を決定する
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void DecideNextAction(Player& pPlayer);

	/// <summary>
	/// バトル中の行動を更新する
	/// </summary>
	void SelectBattleAction(Player& pPlayer);

	/// <summary>
	/// 行動をランダムで決定する
	/// </summary>
	void SelectRandomAction();

	/// <summary>
	/// 状態を更新するかチェックする
	/// </summary>
	/// <returns>状態を更新するかどうか</returns>
	bool IsChangeState();

	/// <summary>
	/// 敵のリストをセットする
	/// </summary>
	/// <param name="enemies"></param>
	void SetEnemyList(const std::vector<std::shared_ptr<EnemyBase>>& enemies) { m_pEnemyList = enemies; }

	/// <summary>
	/// 次の行動を取得する
	/// </summary>
	/// <returns>次の状態名</returns>
	EnemyStateBase::EnemyStateKind GetNextState() const { return m_nextState; }

private:
	std::shared_ptr<EnemyBase> m_pEnemy;						// 敵ポインタ
	std::vector<std::shared_ptr<EnemyBase>> m_pEnemyList;		// 敵のリスト
	std::map<EnemyStateBase::EnemyStateKind, int> m_priority;	// 攻撃の優先度
	EnemyStateBase::EnemyStateKind m_nextState;					// 次の状態

	int m_decisionFrame; // 行動を決定するフレーム数
};
