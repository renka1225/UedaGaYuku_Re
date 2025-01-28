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
	/// <param name="enemyIndex">敵の種類</param>
	void Init(int enemyIndex);

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
	/// <summary>
	/// 敵AIのデータを読み込む
	/// </summary>
	/// <param name="enemyIndex">敵の種類</param>
	void LoadAIData(int enemyIndex);

	/// <summary>
	/// 行動優先度を決める
	/// </summary>
	void DecidePriority(Player& pPlayer);

	/// <summary>
	/// バトル中の場合の行動優先度を決める
	/// </summary>
	void DecideBattlePriority(Player& pPlayer);

	/// <summary>
	/// 優先度を更新する
	/// </summary>
	/// <param name="state">追加する状態</param>
	/// <param name="value">優先度</param>
	void UpdatePriority(EnemyStateBase::EnemyStateKind state, int value);

	/// <summary>
	/// 優先度をリセットする
	/// </summary>
	void ResetPriority();

	/// <summary>
	/// 待機の優先度を追加する
	/// </summary>
	void AddIdlePriority();

	/// <summary>
	/// 移動系の優先度を追加する
	/// </summary>
	void AddMovePriority();

	/// <summary>
	/// 攻撃系の優先度を追加する
	/// </summary>
	void AddAttackPriority();

	/// <summary>
	/// 行動優先度をランダムで追加する
	/// </summary>
	void AddRandomPriority();

	/// <summary>
	/// 敵のIDを取得する
	/// </summary>
	/// <param name="enemyIndex">使用する敵番号</param>
	/// <returns>敵ID</returns>
	std::string GetEnemyId(int enemyIndex);

private:
	// 優先度をリセットする
	struct ActionProbability
	{
		int idle = 0;	// 待機
		int walk = 0;	// 歩き
		int run = 0;	// 走り
		int punch = 0;	// パンチ
		int kick = 0;	// キック
		int avoid = 0;	// 回避
		int guard = 0;	// ガード
	};
	std::map<std::string, ActionProbability> m_acitonProbaility;

	std::shared_ptr<EnemyBase> m_pEnemy;						// 敵ポインタ
	std::vector<std::shared_ptr<EnemyBase>> m_pEnemyList;		// 敵のリスト
	std::map<EnemyStateBase::EnemyStateKind, int> m_priority;	// 行動の優先度
	EnemyStateBase::EnemyStateKind m_nextState;					// 次の状態
	EnemyStateBase::EnemyStateKind m_prevState;					// 1つ前の状態
	
	std::string m_charaId;	// キャラのID
	int m_decisionFrame;	// 行動を決定するフレーム数
};
