#include "Player.h"
#include "EnemyBase.h"
#include "EnemyAI.h"

namespace
{
	constexpr int kDecisionFrame = 60; // 行動を更新する時間
}

EnemyAI::EnemyAI():
	m_pEnemy(nullptr),
	m_nextState(EnemyStateBase::EnemyStateKind::kIdle),
	m_decisionFrame(0)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::Init(std::shared_ptr<EnemyBase> pEnemy)
{
	m_pEnemy = pEnemy;
}

void EnemyAI::Update()
{
	m_decisionFrame--;
}

void EnemyAI::Draw()
{
}

void EnemyAI::DecideNextAction(const Player& pPlayer)
{
	// ダメージを受けている最中は更新しない
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return;

	if (m_decisionFrame > 0) return;
	m_decisionFrame = kDecisionFrame;

	m_candidateState.clear();

	// 他の敵の状態
	int attackEnemyNum = 0;
	for (const auto& enemy : m_pEnemyList)
	{
		if (enemy->GetCurrentAnim() == AnimName::kPunch1 ||
			enemy->GetCurrentAnim() == AnimName::kPunch2 ||
			enemy->GetCurrentAnim() == AnimName::kPunch3 ||
			enemy->GetCurrentAnim() == AnimName::kKick)
		{
			attackEnemyNum++;
		}
	}

	// プレイヤーとの距離
	float dist = VSize(m_pEnemy->GetEToPVec());
	// プレイヤーの状態
	auto playerState = pPlayer.GetCurrentAnim();

	/*MEMO*/
	// 他2体が攻撃中の場合は待機か移動のみ
	// 1体のみ攻撃中の場合はランダム
	// 1体も攻撃していない場合は攻撃優先

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
		if (playerState == AnimName::kPunch1 || playerState == AnimName::kPunch2 || playerState == AnimName::kPunch3 || playerState == AnimName::kKick)
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
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kIdle, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kWalk, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kRun, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kPunch, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kKick, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kAvoid, 10);
			m_candidateState.emplace_back(EnemyStateBase::EnemyStateKind::kGuard, 10);
		}
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