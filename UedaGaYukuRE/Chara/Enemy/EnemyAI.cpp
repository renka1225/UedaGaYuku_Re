#include "Player.h"
#include "EnemyBase.h"
#include "EnemyAI.h"
#include <fstream>
#include <sstream>

namespace
{
	// キャラID
	const std::map<int, std::string> kCharaId
	{
		{CharacterBase::kEnemy_boss, "BOSS"},
		{CharacterBase::kEnemy_tuto, "TUTO"},
		{CharacterBase::kEnemy_bob, "BOSS"},
		{CharacterBase::kEnemy_sato, "SATO"},
		{CharacterBase::kEnemy_abe, "ABE"}
	};
	const std::string kDefaultId = "DEFAULT";	// デフォルトID

	constexpr float kMinApproachRange = 30.0f;	// プレイヤーに近づく最小範囲
	constexpr float kMinChaseRange = 200.0f;	// プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 800.0f;	// プレイヤーを追いかける最大範囲
	constexpr int kDecisionFrame = 30;			// 行動を更新する時間
	constexpr int kMaxAttackEnemyNum = 2;		// 1度に攻撃する敵数
	constexpr float kProbabilityRate = 0.7f;	// 確率を減少させる割合
	
	const char* kLoadFilePath = "data/csv/enemyAI.csv";

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

EnemyAI::EnemyAI(std::shared_ptr<EnemyBase> pEnemy):
	m_pEnemy(pEnemy),
	m_nextState(EnemyStateBase::EnemyStateKind::kIdle),
	m_prevState(m_nextState),
	m_charaId(""),
	m_decisionFrame(0)
{
	for (auto& pair : m_priority)
	{
		pair.second = 0;
	}
}

void EnemyAI::Init(int enemyIndex)
{
	LoadAIData(enemyIndex);
}

void EnemyAI::Update()
{
	m_decisionFrame--;
	m_decisionFrame = std::max(0, m_decisionFrame);
}

void EnemyAI::DecideNextAction(Player& pPlayer)
{
	// 特定の状態中は更新しない
	if (IsChangeState()) return;

	if (m_decisionFrame > 0) return;
	m_decisionFrame = kDecisionFrame;

	DecidePriority(pPlayer);

	// 優先度から次の行動を決定する
	int totalPriority = 0;
	for (const auto& pair : m_priority)
	{
		totalPriority += pair.second;
	}

	int randomValue = GetRand(totalPriority - 1);
	int currenSum = 0;
	for (const auto& pair : m_priority)
	{
		currenSum += pair.second;

		if (randomValue < currenSum)
		{
			m_prevState = m_nextState;
			m_nextState = pair.first;
#ifdef _DEBUG
			printfDx("次行動:%d\n", m_nextState);
#endif
			break;
		}
	}
}

void EnemyAI::DecidePriority(Player& pPlayer)
{
	for (auto& pair : m_priority)
	{
		pair.second = 0;
	}

	float dist = VSize(m_pEnemy->GetEToPVec());	// 敵からプレイヤーまでの距離

	// バトル中でない場合
	if (!pPlayer.GetIsBattle())
	{
		// プレイヤーを追いかける範囲内に入っている場合
		bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange;
		if (isChaseRange)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kRun] += m_probability.highProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.mediumProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.lowProbability;
		}
		else
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.highProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.lowProbability;

			pPlayer.SetIsBattle(false);
		}
	}
	// バトル中の場合
	else
	{
		if (IsChangeState()) return;

		// チュートリアル敵
		if (m_pEnemy->GetEnemyIndex() == CharacterBase::CharaType::kEnemy_tuto)
		{
			DecideTutoPriority();
		}
		// それ以外
		else
		{
			DecideBattlePriority(pPlayer);
		}
	}
}

void EnemyAI::DecideBattlePriority(Player& pPlayer)
{
	/*MEMO*/
	// 他2体が攻撃中の場合は待機か移動のみ
	// 1体のみ攻撃中の場合はランダム
	// 1体も攻撃していない場合は攻撃優先

	float dist = VSize(m_pEnemy->GetEToPVec());  // プレイヤーとの距離
	auto playerState = pPlayer.GetCurrentAnim(); // プレイヤーの状態

	// 他の敵の攻撃状態を取得
	int attackEnemyNum = 0;
	for (const auto& enemy : m_pEnemyList)
	{
		if (enemy == nullptr) continue;

		if (enemy->GetIsAttack())
		{
			attackEnemyNum++;
		}
#ifdef _DEBUG
		printfDx("攻撃中敵数:%d\n", attackEnemyNum);
#endif
	}
	
	// バトル中プレイヤーから離れた場合
	if (pPlayer.GetIsBattle() && dist >= kMaxChaseRange)
	{
		// バトルを終了状態にする
		pPlayer.SetIsBattle(false);
		return;
	}

	// プレイヤーとの距離が離れている場合
	if (dist > kMinChaseRange)
	{
		// 歩きor走りでプレイヤーに近づく
		m_priority[EnemyStateBase::EnemyStateKind::kRun] += m_probability.lowProbability;
	}
	// 距離が近い場合
	else
	{
		// 他の敵が攻撃中の場合
		if (attackEnemyNum >= kMaxAttackEnemyNum)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 0;
		}
		else if (attackEnemyNum == 0)
		{
			// 攻撃優先
			m_priority[EnemyStateBase::EnemyStateKind::kPunch] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kKick] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kRun] += m_probability.veryLowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += m_probability.veryLowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kGuard] += m_probability.veryLowProbability;
		}
		else
		{
			// ランダム
			DecideRandomPriority();
		}

		// プレイヤーが攻撃中の場合
		if (pPlayer.GetIsAttack())
		{
			m_priority[EnemyStateBase::EnemyStateKind::kGuard] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += m_probability.veryLowProbability;
		}
		// プレイヤーが回避中の場合
		else if (playerState == AnimName::kAvoid)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kPunch] += m_probability.veryLowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kKick] += m_probability.veryLowProbability;
		}
		// プレイヤーがガード中の場合
		else if (playerState == AnimName::kGuard)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.lowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kPunch] += m_probability.veryLowProbability;
			m_priority[EnemyStateBase::EnemyStateKind::kKick] += m_probability.veryLowProbability;
		}
		// その他
		else
		{
			// ランダム
			DecideRandomPriority();
		}
	}

	// プレイヤーに一定距離近づいた場合
	if (dist <= kMinApproachRange)
	{
		// 移動しないようにする
		m_priority[EnemyStateBase::EnemyStateKind::kRun] = 0;
		m_priority[EnemyStateBase::EnemyStateKind::kWalk] = 0;
		m_priority[EnemyStateBase::EnemyStateKind::kIdle] = m_probability.veryLowProbability;
	}

	// 連続して攻撃する確率を減らす
	if (m_prevState == EnemyStateBase::EnemyStateKind::kPunch || m_prevState == EnemyStateBase::EnemyStateKind::kKick || m_prevState == EnemyStateBase::EnemyStateKind::kAvoid)
	{
		if (m_priority.find(m_prevState) != m_priority.end())
		{
			m_priority[m_prevState] = static_cast<int>(std::max(0, m_priority[m_prevState] - m_probability.veryLowProbability) * kProbabilityRate);
#ifdef _DEBUG
			printfDx("%dの攻撃確率減少:%d\n", static_cast<int>(m_prevState), m_priority[m_prevState]);
#endif
		}
	}
}

void EnemyAI::DecideTutoPriority()
{
	m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.highProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.highProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kRun] += m_probability.lowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kPunch] += m_probability.veryLowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kKick] += m_probability.veryLowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += m_probability.lowProbability;

	float dist = VSize(m_pEnemy->GetEToPVec());  // プレイヤーとの距離
	// プレイヤーに一定距離近づいた場合
	if (dist <= kMinApproachRange)
	{
		// 移動しないようにする
		m_priority[EnemyStateBase::EnemyStateKind::kRun] = 0;
		m_priority[EnemyStateBase::EnemyStateKind::kWalk] = 0;
		m_priority[EnemyStateBase::EnemyStateKind::kIdle] = m_probability.veryLowProbability;
	}
}

void EnemyAI::DecideRandomPriority()
{
	m_priority[EnemyStateBase::EnemyStateKind::kIdle] += m_probability.mediumProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kWalk] += m_probability.mediumProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kRun] += m_probability.lowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kPunch] += m_probability.lowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kKick] += m_probability.lowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += m_probability.lowProbability;
	m_priority[EnemyStateBase::EnemyStateKind::kGuard] += m_probability.lowProbability;
}

bool EnemyAI::IsChangeState()
{
	if (m_pEnemy->GetIsAttack()) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDown) return true;
	if(m_pEnemy->GetCurrentAnim() == AnimName::kAvoid) return true;
	if(m_pEnemy->GetCurrentAnim() == AnimName::kGuard) return true;

	return false;
}

void EnemyAI::LoadAIData(int enemyIndex)
{
	std::ifstream ifs(kLoadFilePath);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		
		// ID名を取得
		m_charaId = GetEnemyId(enemyIndex);

		if (strvec[0] == m_charaId)
		{
			try
			{
				m_probability.veryLowProbability = stoi(strvec[1]);
				m_probability.lowProbability = stoi(strvec[2]);
				m_probability.mediumProbability = stoi(strvec[3]);
				m_probability.highProbability = stoi(strvec[4]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}
}

std::string EnemyAI::GetEnemyId(int enemyIndex)
{
	if (enemyIndex == CharacterBase::kEnemy_boss)
	{
		return kCharaId.at(CharacterBase::kEnemy_boss);
	}
	else if(enemyIndex == CharacterBase::kEnemy_tuto)
	{
		return kCharaId.at(CharacterBase::kEnemy_tuto);
	}
	else if (enemyIndex == CharacterBase::kEnemy_bob)
	{
		return kCharaId.at(CharacterBase::kEnemy_bob);
	}
	else if (enemyIndex == CharacterBase::kEnemy_sato)
	{
		return kCharaId.at(CharacterBase::kEnemy_sato);
	}
	else if (enemyIndex == CharacterBase::kEnemy_abe)
	{
		return kCharaId.at(CharacterBase::kEnemy_abe);
	}

	return kDefaultId;
}
