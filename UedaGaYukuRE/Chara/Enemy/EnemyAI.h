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
	/// <param name="pEnemy">ポインタ</param>
	EnemyAI(std::shared_ptr<EnemyBase> pEnemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyAI();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 次の行動を決定する
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void DecideNextAction(const Player& pPlayer, const std::vector<std::shared_ptr<EnemyBase>>& pEnemy);

	/// <summary>
	/// 次の行動を取得する
	/// </summary>
	/// <returns>次の状態名</returns>
	EnemyStateBase::EnemyStateKind GetNextState() const { return m_nextState; }

private:
	std::shared_ptr<EnemyBase> m_pEnemy;		// 敵ポインタ
	EnemyStateBase::EnemyStateKind m_nextState; // 次の状態
	std::vector<std::pair<EnemyStateBase::EnemyStateKind, int>> m_candidateState; // 行動の候補を保存する
	int m_decisionFrame;						// 行動を決定するフレーム数
};
