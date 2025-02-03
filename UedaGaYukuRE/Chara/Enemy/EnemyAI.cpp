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
		{CharacterBase::kEnemy_bob, "BOB"},
		{CharacterBase::kEnemy_sato, "SATO"},
		{CharacterBase::kEnemy_abe, "ABE"}
	};
	const std::string kDefaultId = "DEFAULT";	// デフォルトID

	constexpr float kMinApproachRange = 30.0f;	// プレイヤーに近づく最小範囲
	constexpr float kMinChaseRange = 200.0f;	// プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 800.0f;	// プレイヤーを追いかける最大範囲
	constexpr int kDecisionFrame = 10;			// 行動を更新する時間
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
	ResetPriority();
}

void EnemyAI::Init(int enemyIndex)
{
	LoadAIData(enemyIndex);
}

void EnemyAI::Update()
{
	m_decisionFrame--;
	m_decisionFrame = std::max(0, m_decisionFrame);

#ifdef _DEBUG
	/*for (const auto& pair : m_priority)
	{
		printfDx("State:%d, Priority:%d\n", pair.first, pair.second);
	}*/
#endif
}

void EnemyAI::DecideNextAction(Player& pPlayer)
{
	// 特定の状態中は更新しない
	if (IsChangeState()) return;

	// 数フレームごとに更新する
	if (m_decisionFrame > 0) return;
	m_decisionFrame = kDecisionFrame;

	DecidePriority(pPlayer); // 行動優先度を決める

	// 優先度から次の行動を決定する
	int totalPriority = 0;
	for (const auto& pair : m_priority)
	{
		totalPriority += pair.second;
	}

	if (totalPriority == 0) return; // 優先度が全て0の場合は行動しない

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

void EnemyAI::LoadAIData(int enemyIndex)
{
	std::ifstream ifs(kLoadFilePath);
	std::string line;
	std::vector<std::string> strvec;

	while (std::getline(ifs, line))
	{
		m_charaId = GetEnemyId(enemyIndex); // ID名を取得

		strvec = split(line, ',');
		if (strvec[0] == m_charaId)
		{
			try
			{
				m_acitonProbaility[m_charaId].idle = stoi(strvec[1]);
				m_acitonProbaility[m_charaId].walk = stoi(strvec[2]);
				m_acitonProbaility[m_charaId].run = stoi(strvec[3]);
				m_acitonProbaility[m_charaId].punch = stoi(strvec[4]);
				m_acitonProbaility[m_charaId].kick = stoi(strvec[5]);
				m_acitonProbaility[m_charaId].avoid = stoi(strvec[6]);
				m_acitonProbaility[m_charaId].guard = stoi(strvec[7]);
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}
}

void EnemyAI::DecidePriority(Player& pPlayer)
{
	// 確率をリセット
	ResetPriority();

	float dist = VSize(m_pEnemy->GetEToPVec());	// 敵からプレイヤーまでの距離
	bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange; // プレイヤーを追いかける範囲内に入っているかどうか

	// バトル中でない場合
	if (!pPlayer.GetIsBattle())
	{
		// 範囲内の場合
		if (isChaseRange)
		{
			AddMovePriority();
		}
		// 離れた場合
		else
		{
			UpdatePriority(EnemyStateBase::EnemyStateKind::kIdle, m_acitonProbaility[m_charaId].idle);
			UpdatePriority(EnemyStateBase::EnemyStateKind::kWalk, m_acitonProbaility[m_charaId].walk);
		}
	}
	// バトル中の場合
	else
	{
		if (IsChangeState()) return;
		DecideBattlePriority(pPlayer);
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

	// 攻撃中の敵数を取得
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

	// プレイヤーに一定距離近づいた場合
	if (dist < kMinApproachRange)
	{
		AddRandomPriority();
		// 移動しない
		AddIdlePriority();
	}
	// プレイヤーとの距離が離れている場合
	else if (dist > kMinChaseRange)
	{
		// プレイヤーに近づく
		AddMovePriority();
	}
	else
	{
		// 他の敵が攻撃中の場合
		if (attackEnemyNum >= kMaxAttackEnemyNum)
		{
			UpdatePriority(EnemyStateBase::EnemyStateKind::kIdle, m_acitonProbaility[m_charaId].idle);
			UpdatePriority(EnemyStateBase::EnemyStateKind::kWalk, m_acitonProbaility[m_charaId].walk);
			UpdatePriority(EnemyStateBase::EnemyStateKind::kRun, m_acitonProbaility[m_charaId].run);
		}
		// 1体も攻撃していない場合
		else if (attackEnemyNum == 0)
		{
			// 攻撃優先
			AddAttackPriority();
		}
		else
		{
			// ランダム
			AddRandomPriority();
		}

		// プレイヤーが攻撃中の場合
		if (pPlayer.GetIsAttack())
		{
			UpdatePriority(EnemyStateBase::EnemyStateKind::kGuard, m_acitonProbaility[m_charaId].guard);
			UpdatePriority(EnemyStateBase::EnemyStateKind::kAvoid, m_acitonProbaility[m_charaId].avoid);
		}
		// プレイヤーが回避中の場合
		else if (playerState == AnimName::kAvoid)
		{
			AddMovePriority();
			AddAttackPriority();
		}
		// プレイヤーがガード中の場合
		else if (playerState == AnimName::kGuard)
		{
			AddMovePriority();
		}
		// その他
		else
		{
			// ランダム
			AddRandomPriority();
		}
	}

	// 連続して攻撃する確率を減らす
	if (m_prevState == EnemyStateBase::EnemyStateKind::kPunch || m_prevState == EnemyStateBase::EnemyStateKind::kKick || m_prevState == EnemyStateBase::EnemyStateKind::kAvoid)
	{
		if (m_priority.find(m_prevState) != m_priority.end())
		{
			m_priority[m_prevState] = static_cast<int>(m_priority[m_prevState] * kProbabilityRate);
#ifdef _DEBUG
			printfDx("%dの攻撃確率減少:%d\n", static_cast<int>(m_prevState), m_priority[m_prevState]);
#endif
		}
	}
}

bool EnemyAI::IsChangeState()
{
	if (m_pEnemy->GetIsAttack()) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDown) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kAvoid) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kGuard) return true;

	return false;
}

void EnemyAI::UpdatePriority(EnemyStateBase::EnemyStateKind state, int value)
{
	if (m_priority.find(state) != m_priority.end())
	{
		m_priority[state] += value;
	}
}

void EnemyAI::ResetPriority()
{
	for (auto& pair : m_priority)
	{
		pair.second = m_acitonProbaility[m_charaId].idle;
	}

	m_priority[EnemyStateBase::EnemyStateKind::kIdle] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kWalk] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kRun] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kPunch] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kKick] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kAvoid] = 0;
	m_priority[EnemyStateBase::EnemyStateKind::kGuard] = 0;
}

void EnemyAI::AddIdlePriority()
{
	UpdatePriority(EnemyStateBase::EnemyStateKind::kRun, 0);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kWalk, 0);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kIdle, m_acitonProbaility[m_charaId].idle);
}

void EnemyAI::AddMovePriority()
{
	UpdatePriority(EnemyStateBase::EnemyStateKind::kWalk, m_acitonProbaility[m_charaId].walk);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kRun, m_acitonProbaility[m_charaId].run);
}

void EnemyAI::AddAttackPriority()
{
	UpdatePriority(EnemyStateBase::EnemyStateKind::kPunch, m_acitonProbaility[m_charaId].punch);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kKick, m_acitonProbaility[m_charaId].kick);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kRun, m_acitonProbaility[m_charaId].run);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kAvoid, m_acitonProbaility[m_charaId].avoid);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kGuard, m_acitonProbaility[m_charaId].guard);
}

void EnemyAI::AddRandomPriority()
{
	UpdatePriority(EnemyStateBase::EnemyStateKind::kIdle, m_acitonProbaility[m_charaId].idle);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kWalk, m_acitonProbaility[m_charaId].walk);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kRun, m_acitonProbaility[m_charaId].run);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kPunch, m_acitonProbaility[m_charaId].punch);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kKick, m_acitonProbaility[m_charaId].kick);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kAvoid, m_acitonProbaility[m_charaId].avoid);
	UpdatePriority(EnemyStateBase::EnemyStateKind::kGuard, m_acitonProbaility[m_charaId].guard);
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
