#include "Player.h"
#include "EnemyBase.h"
#include "EnemyAI.h"

namespace
{
	constexpr float kMinChaseRange = 200.0f; // プレイヤーを追いかける最小範囲
	constexpr float kMaxChaseRange = 800.0f; // プレイヤーを追いかける最大範囲

	constexpr int kDecisionFrame = 60; // 行動を更新する時間
}

EnemyAI::EnemyAI(std::shared_ptr<EnemyBase> pEnemy):
	m_pEnemy(pEnemy),
	m_nextState(EnemyStateBase::EnemyStateKind::kIdle),
	m_decisionFrame(0)
{
}

void EnemyAI::Update()
{
	m_decisionFrame--;
}

void EnemyAI::DecideNextAction(const Player& pPlayer)
{
	// 特定の状態中は更新しない
	if (IsChangeState()) return;

	if (m_decisionFrame > 0) return;
	m_decisionFrame = kDecisionFrame;

	m_candidateState.clear();

	// バトル中でない場合
	if (!pPlayer.GetIsBattle())
	{
		float dist = VSize(m_pEnemy->GetEToPVec());	// 敵からプレイヤーまでの距離

		// プレイヤーを追いかける範囲内に入っている場合
		bool isChaseRange = dist > kMinChaseRange && dist < kMaxChaseRange;
		if (isChaseRange)
		{
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kRun, 100);
		}
		else
		{
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 70);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kWalk, 30);
		}
	}
	// バトル中の場合
	else
	{
		SelectBattleAction(pPlayer);
	}
	

	// 候補がない場合は待機状態にする
	if (m_candidateState.empty())
	{
		m_nextState = EnemyStateBase::EnemyStateKind::kIdle;
		return;
	}

	// 候補から次の行動をランダムで決定する
	int randTotal = 0;
	for (const auto& state : m_candidateState)
	{
		randTotal += state.second;
	}

	// ランダム値を生成
	int randState = GetRand(randTotal - 1);

	int currentRand = 0;
	for (const auto& state : m_candidateState)
	{
		currentRand += state.second;
		if (randState < currentRand)
		{
			m_nextState = state.first;
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
	}

	// プレイヤーとの距離が離れている場合
	if (dist > 100)
	{
		// 歩きor走りでプレイヤーに近づく
		m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kWalk, 30);
		m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kRun, 70);
		m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 10);
	}
	// 距離が近い場合
	else
	{

		// 他の敵が攻撃中の場合
		if (attackEnemyNum >= 2)
		{
			// 待機または移動優先
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 50);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kWalk, 30);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 20);

			printfDx("状態1\n");
		}
		else if (attackEnemyNum == 1)
		{
			// ランダム行動
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 40);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 30);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 30);
			printfDx("状態2\n");
		}
		else
		{
			// 攻撃優先
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 70);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 50);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 10);
			printfDx("状態3\n");
		}

		// プレイヤーが攻撃中の場合
		if (pPlayer.GetIsAttack())
		{
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 30);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 50);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 50);
		}
		// プレイヤーが回避中の場合
		else if (playerState == AnimName::kAvoid)
		{
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 70);
		}
		// プレイヤーがガード中の場合
		else if (playerState == AnimName::kGuard)
		{
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 70);
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
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kWalk, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kRun, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 10);
	m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 10);
}

bool EnemyAI::IsChangeState()
{
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return true;
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDown) return true;

	return false;
}
