#include "Player.h"
#include "EnemyBase.h"
#include "EnemyAI.h"

namespace
{
	constexpr float kMinChaseRange = 200.0f; // プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 800.0f; // プレイヤーを追いかける最大範囲
	constexpr int kDecisionFrame = 180; // 行動を更新する時間
}

EnemyAI::EnemyAI(std::shared_ptr<EnemyBase> pEnemy):
	m_pEnemy(pEnemy),
	m_nextState(EnemyStateBase::EnemyStateKind::kIdle),
	m_decisionFrame(0)
{
	for (auto& pair : m_priority)
	{
		pair.second = 0;
	}
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
			m_priority[EnemyStateBase::EnemyStateKind::kRun] += 50;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 20;
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 10;
		}
		else
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 70;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 10;

			pPlayer.SetIsBattle(false);
		}
	}
	// バトル中の場合
	else
	{
		if (!IsChangeState())
		{
			SelectBattleAction(pPlayer);
			printfDx("状態更新\n");
		}
	}

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
			m_nextState = pair.first;
			printfDx("次行動:%d\n", m_nextState);
			break;
		}
	}
}

void EnemyAI::SelectBattleAction(const Player& pPlayer)
{
	/*MEMO*/
	// 他2体が攻撃中の場合は待機か移動のみ
	// 1体のみ攻撃中の場合はランダム
	// 1体も攻撃していない場合は攻撃優先

	// プレイヤーとの距離
	float dist = VSize(m_pEnemy->GetEToPVec());
	// プレイヤーの状態
	auto playerState = pPlayer.GetCurrentAnim();

	// 他の敵の攻撃状態を取得
	int attackEnemyNum = 0;
	for (const auto& enemy : m_pEnemyList)
	{
		if (enemy == nullptr) continue;

		if (enemy->GetIsAttack())
		{
			attackEnemyNum++;
		}
		printfDx("攻撃中敵数:%d\n", attackEnemyNum);
	}

	// プレイヤーとの距離が離れている場合
	if (dist > kMinChaseRange)
	{
		// 歩きor走りでプレイヤーに近づく
		m_priority[EnemyStateBase::EnemyStateKind::kRun] += 10;
	}
	// 距離が近い場合
	else
	{
		// 他の敵が攻撃中の場合
		if (attackEnemyNum >= 2)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 10;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 10;
		}
		else if (attackEnemyNum == 1)
		{
			// ランダム行動
			SelectRandomAction();
		}
		else
		{
			// 攻撃優先
			m_priority[EnemyStateBase::EnemyStateKind::kPunch] += 10;
			m_priority[EnemyStateBase::EnemyStateKind::kKick] += 10;
		}

		// プレイヤーが攻撃中の場合
		if (pPlayer.GetIsAttack())
		{
			m_priority[EnemyStateBase::EnemyStateKind::kGuard] += 10;
			m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += 10;
		}
		// プレイヤーが回避中の場合
		else if (playerState == AnimName::kAvoid)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 10;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 10;
		}
		// プレイヤーがガード中の場合
		else if (playerState == AnimName::kGuard)
		{
			m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 10;
			m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 10;
		}
		// プレイヤーのHPが高い場合
		//else if (pPlayer.GetHp() >= 100.0f)
		//{
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 10);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 30);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 50);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 40);
		//}
		//// プレイヤーのHPが低い場合
		//else if (pPlayer.GetHp() <= 50.0f)
		//{
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 10);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 10);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 70);
		//	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 30);
		//}
		// その他
		else
		{
			SelectRandomAction();
		}
	}
}

void EnemyAI::SelectRandomAction()
{
	m_priority[EnemyStateBase::EnemyStateKind::kIdle] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kWalk] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kRun] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kPunch] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kKick] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kAvoid] += 10;
	m_priority[EnemyStateBase::EnemyStateKind::kGuard] += 10;
}

bool EnemyAI::IsChangeState()
{
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDown) return true;

	if (m_pEnemy->GetIsAttack()) return true;
	if(m_pEnemy->GetCurrentAnim() == AnimName::kAvoid) return true;
	if(m_pEnemy->GetCurrentAnim() == AnimName::kGuard) return true;

	return false;
}