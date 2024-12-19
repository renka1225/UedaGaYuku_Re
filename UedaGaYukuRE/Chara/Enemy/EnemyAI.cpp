#include "Player.h"
#include "EnemyBase.h"
#include "EnemyAI.h"

namespace
{
	constexpr int kDecisionFrame = 60; // 行動を更新する時間
}

EnemyAI::EnemyAI(std::shared_ptr<EnemyBase> pEnemy):
	m_pEnemy(pEnemy),
	m_nextState(EnemyStateBase::EnemyStateKind::kIdle),
	m_decisionFrame(0)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::Init()
{
}

void EnemyAI::Update()
{
	m_decisionFrame--;
}

void EnemyAI::Draw()
{
}

void EnemyAI::DecideNextAction(const Player& pPlayer, const std::vector<std::shared_ptr<EnemyBase>>& pEnemy)
{
	// ダメージを受けている最中は更新しない
	if (m_pEnemy->GetCurrentAnim() == AnimName::kDamage) return;

	if (m_decisionFrame > 0) return;
	m_decisionFrame = kDecisionFrame;

	// プレイヤーとの距離
	float dist = VSize(m_pEnemy->GetEToPVec());

	// プレイヤーの状態
	auto playerState = pPlayer.GetCurrentAnim();

	// 他2体が攻撃中の場合は待機か移動のみ
	// 1体のみ攻撃中の場合はランダム
	// 1体も攻撃していない場合は攻撃優先

	// プレイヤーとの距離が離れている場合
	if (dist > 100)
	{
		// 歩きor走りでプレイヤーに近づく
	}
	// 距離が近い場合
	else
	{
		// プレイヤーのHPが高い場合
		if (pPlayer.GetHp() >= 100.0f)
		{

		}
		// プレイヤーのHPが低い場合
		else if (pPlayer.GetHp() <= 50.0f)
		{

		}
		// プレイヤーが攻撃中の場合
		else if (playerState == AnimName::kPunch1 || playerState == AnimName::kPunch2 || playerState == AnimName::kPunch3 || playerState == AnimName::kKick)
		{

		}
		// プレイヤーが回避中の場合
		else if (playerState == AnimName::kAvoid)
		{

		}
		// プレイヤーがガード中の場合
		else if (playerState == AnimName::kGuard)
		{

		}
		// その他
		else
		{
			m_nextState = EnemyStateBase::EnemyStateKind::kPunch;
		}
	}
}

EnemyStateBase::EnemyStateKind EnemyAI::GetNextState() const
{
	return m_nextState;
}