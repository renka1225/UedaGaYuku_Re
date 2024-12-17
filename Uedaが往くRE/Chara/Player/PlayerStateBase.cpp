#include "Input.h"
#include "Sound.h"
#include "EffectManager.h"
#include "SceneBase.h"
#include "Weapon.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateGuard.h"
#include "PlayerStateGrab.h"
#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(std::shared_ptr<Player> pPlayer):
	m_upMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_leftMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_moveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_animEndTime(0.0f),
	m_analogInput({}),
	m_analogX(0),
	m_analogY(0)
{
	m_pPlayer = pPlayer;
}

void PlayerStateBase::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	GetJoypadAnalogInput(&m_analogX, &m_analogY, DX_INPUT_PAD1); // アナログスティックの入力状態
	m_pPlayer->Move(m_moveVec, stage, false);   // 移動情報を反映する

	// 特定の状態中は更新しない
	if (IsStateInterrupt()) return;

	// 移動できない状態の場合
	if (!m_pPlayer->GetIsPossibleMove())
	{
		ChangeStateIdle();
		return;
	}

	// バトル中でない場合は以下の処理はできないようにする
	if (!m_pPlayer->GetIsBattle()) return;

	// ゲージが最大まで溜まっているか
	bool isSpecial = m_pPlayer->GetGauge() >= m_pPlayer->GetStatus().maxGauge;
	// 敵が範囲内にいるか
	bool isRange = m_pPlayer->GetIsPossibleGrabEnemy();

	// 攻撃のボタンが押された場合
	if (input.IsTriggered(InputId::kPunch) || input.IsTriggered(InputId::kKick))
	{
		ChangeStateAttack(input);
		return;
	}
	// ガードのボタンが押された場合
	else if (input.IsPressing(InputId::kGuard))
	{
		ChangeStateGuard();
		return;
	}
	// 回避のボタンが押された場合
	else if (input.IsTriggered(InputId::kAvoid))
	{
		ChangeStateAvoid();
		return;
	}
	// 掴みのボタンが押された場合
	else if (input.IsTriggered(InputId::kGrab))
	{
		ChangeStateGrab(weapon);
		return;
	}
	// 必殺技のボタンが押された場合
	else if (input.IsTriggered(InputId::kSpecial))
	{
		// 条件を満たしていれば必殺技を発動する
		if (isSpecial && isRange)
		{
			ChangeStateSpecialAttack();

			// ゲージを減らす
			m_pPlayer->SetGauge(0.0f);
		}
		return;
	}

	// ダメージを受けた場合
	else if (m_pPlayer->GetIsOnDamage())
	{
		ChangeStateDamage();
		return;
	}
}

bool PlayerStateBase::IsStateInterrupt()
{
	// 回避中
	if (GetKind() == PlayerStateKind::kAvoid) return true;
	// ダメージを受けている途中
	if (GetKind() == PlayerStateKind::kDamage) return true;
	// 攻撃中
	if (GetKind() == PlayerStateKind::kAttack) return true;

	return false;
}

void PlayerStateBase::ChangeStateIdle()
{
	// すでに待機状態の場合は飛ばす
	if (GetKind() == PlayerStateKind::kIdle) return;

	// StateをIdleに変更する
	std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateAttack(const Input& input)
{
	// 攻撃中は再度攻撃できないようにする
	if (m_pPlayer->GetIsAttack()) return;

	// StateをAttackに変更する
	m_pPlayer->SetIsAttack(true);
	std::shared_ptr<PlayerStateAttack> state = std::make_shared<PlayerStateAttack>(m_pPlayer);
	m_nextState = state;

	// 押されたボタンによって状態を変更する
	if (input.IsTriggered(InputId::kPunch))
	{
		state->Init(AnimName::kPunch1);
	}
	else if (input.IsTriggered(InputId::kKick))
	{
		state->Init(AnimName::kKick);
	}
}

void PlayerStateBase::ChangeStateSpecialAttack()
{
	m_pPlayer->SetIsAttack(true);
	std::shared_ptr<PlayerStateAttack> state = std::make_shared<PlayerStateAttack>(m_pPlayer);
	m_nextState = state;
	state->Init(AnimName::kKickHeat);
}

void PlayerStateBase::ChangeStateGuard()
{
	// StateをGuardに変更する
	m_pPlayer->SetIsGuard(true);
	std::shared_ptr<PlayerStateGuard> state = std::make_shared<PlayerStateGuard>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateAvoid()
{
	Sound::GetInstance().PlaySe(SoundName::kSe_avoid);

	// StateをAvoidに変更する
	std::shared_ptr<PlayerStateAvoid> state = std::make_shared<PlayerStateAvoid>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateGrab(Weapon& pWeapon)
{
	// すでに武器を掴んでいる場合
	if (m_pPlayer->GetIsGrabWeapon())
	{
		m_pPlayer->SetIsGrabWeapon(false); // 武器を離す
		m_pPlayer->SetIsPossibleGrabWeapon(true);

		pWeapon.UpdateIsGrab(false);
		return;
	}

	// 武器を掴んでいない場合かつ範囲内に武器がある場合
	if (!m_pPlayer->GetIsGrabWeapon() && m_pPlayer->GetIsPossibleGrabWeapon())
	{
		// 拾った武器の情報を取得する
		std::string weaponTag = pWeapon.GetNearWeaponTag();

		m_pPlayer->SetIsGrabWeapon(true);	// 武器を掴む

		// StateをGrabに変更する
		std::shared_ptr<PlayerStateGrab> state = std::make_shared<PlayerStateGrab>(m_pPlayer);
		m_nextState = state;

		// 武器の種類をセットする
		state->Init(weaponTag);

		pWeapon.UpdateIsGrab(true);
		return;
	}

	// 範囲内に敵がいる場合
	if (m_pPlayer->GetIsPossibleGrabEnemy())
	{
		// StateをGrabに変更する
		std::shared_ptr<PlayerStateGrab> state = std::make_shared<PlayerStateGrab>(m_pPlayer);
		m_nextState = state;
		state->Init("enemy");
		return;
	}
}

void PlayerStateBase::ChangeStateDamage()
{
	// StateをHitAttackに変更する
	/*std::shared_ptr<PlayerStateHitAttack> state = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	m_nextState = state;
	state->Init();*/

	// ガード中の場合
	if (m_pPlayer->GetIsGuard())
	{
		Sound::GetInstance().PlaySe(SoundName::kSe_guardAttack);

		// ガードエフェクトを表示
		//EffectManager::GetInstance().Add("guard", m_pPlayer->GetPos());
	}
}
