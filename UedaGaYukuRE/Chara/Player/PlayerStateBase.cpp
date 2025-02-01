#include "Input.h"
#include "Sound.h"
#include "EffectManager.h"
#include "ModelFrameName.h"
#include "SceneBase.h"
#include "Weapon.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateGuard.h"
#include "PlayerStateGrab.h"
#include "PlayerStateDeath.h"
#include "PlayerStateBase.h"

namespace
{
	constexpr float kDamageEffectAdjY = -20.0f; // ダメージエフェクトの表示位置調整
	constexpr float kDecreaseGauge = -100.0f;	// 減らすゲージ量
}

PlayerStateBase::PlayerStateBase(const std::shared_ptr<Player>& pPlayer):
	m_upMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_leftMoveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_moveVec(VGet(0.0f, 0.0f, 0.0f)),
	m_animEndTime(0.0f),
	m_isGuardEffect(false),
	m_isNowBattleEnd(false),
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

	// メニューを開いたとき
	if (input.IsTriggered(InputId::kMenu))
	{
		// ガード状態を解除する
		m_pPlayer->SetIsGuard(false);
		ChangeStateIdle();
	}

	// 死亡処理
	if (m_pPlayer->GetHp() <= 0.0f)
	{
		ChangeStateDeath();
		return;
	}

	// 移動できない場合
	if (!m_pPlayer->GetIsPossibleMove())
	{
		ChangeStateIdle();
		return;
	}

	// ダメージを受けた場合
	if (m_pPlayer->GetIsOnDamage())
	{
		if (m_pPlayer->GetIsGuard())
		{
			// すでに再生中の場合は飛ばす
			if (Sound::GetInstance().GetIsPlaySe(SoundName::kSe_guardAttack)) return;

			Sound::GetInstance().PlaySe(SoundName::kSe_guardAttack);
			EffectManager::GetInstance().Add(EffectName::kGuard, m_pPlayer->GetPos());
			return;
		}
		else
		{
			ChangeStateDamage();
		}
		return;
	}

	// 特定の状態中は更新しない
	if (IsStateInterrupt()) return;

	m_isNowBattleEnd = false;

	// バトル中でない場合
	if (!m_pPlayer->GetIsBattle()) return;

	// 掴みのボタンが押された場合
	if (input.IsTriggered(InputId::kGrab))
	{
		ChangeStateGrab(weapon);
	}

	// 必殺技を出せる状態
	if (m_pPlayer->GetIsSpecial())
	{
		// ボタンが押された場合
		if (input.IsTriggered(InputId::kSpecial))
		{
			ChangeStateSpecialAttack(weapon);
			return;
		}
	}

	// ガードのボタンが押された場合
	if (input.IsPressing(InputId::kGuard))
	{
		ChangeStateGuard();
		return;
	}

	// 攻撃のボタンが押された場合
	if (input.IsTriggered(InputId::kPunch) || input.IsTriggered(InputId::kKick))
	{
		ChangeStateAttack(input);
		return;
	}
	// 回避のボタンが押された場合
	if (input.IsTriggered(InputId::kAvoid))
	{
		ChangeStateAvoid();
		return;
	}
}

void PlayerStateBase::UpdateBattleEnd()
{
	ChangeStateIdle();
	m_isNowBattleEnd = true;
}

bool PlayerStateBase::IsStateInterrupt()
{
	// 回避中
	if (GetKind() == PlayerStateKind::kAvoid) return true;
	// ガード中
	if (GetKind() == PlayerStateKind::kGuard) return true;
	// ダメージを受けている途中
	if (GetKind() == PlayerStateKind::kDamage) return true;
	// 攻撃中
	if (GetKind() == PlayerStateKind::kAttack) return true;
	// 死亡時
	if (GetKind() == PlayerStateKind::kDeath) return true;
	// バトル終了演出中
	if (m_isNowBattleEnd) return true;

	return false;
}

void PlayerStateBase::ChangeStateIdle()
{
	// すでに待機状態の場合は飛ばす
	if (GetKind() == PlayerStateKind::kIdle) return;

	std::shared_ptr<PlayerStateIdle> state = std::make_shared<PlayerStateIdle>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateAttack(const Input& input)
{
	// 攻撃中は再度攻撃できないようにする
	if (m_pPlayer->GetIsAttack()) return;

	// 敵に近づく
	m_pPlayer->AdjPosAttack();

	// StateをAttackに変更する
	m_pPlayer->SetIsAttack(true);
	std::shared_ptr<PlayerStateAttack> state = std::make_shared<PlayerStateAttack>(m_pPlayer);
	m_nextState = state;

	// 押されたボタンによって状態を変更する
	if (input.IsTriggered(InputId::kPunch))
	{
		if (m_pPlayer->GetIsGrabWeapon())
		{
			state->Init(AnimName::kOneHandWeapon);
		}
		else
		{
			state->Init(AnimName::kPunch1);
		}
		
	}
	else if (input.IsTriggered(InputId::kKick))
	{
		state->Init(AnimName::kKick);
	}
}

void PlayerStateBase::ChangeStateSpecialAttack(Weapon& pWeapon)
{
	// 武器を持っている場合は武器を離す
	if (m_pPlayer->GetIsGrabWeapon())
	{
		m_pPlayer->SetIsPossibleGrabWeapon(false);
		m_pPlayer->SetIsGrabWeapon(false);
		pWeapon.UpdateIsGrab(false);
	}

	// エフェクト
	const VECTOR effectPos = m_pPlayer->GetModelFramePos(PlayerFrameName::kRightEnd.c_str());
	EffectManager::GetInstance().Add(EffectName::kSpecialAttack, effectPos, 0.0f, m_pPlayer.get());

	m_pPlayer->UpdateGauge(kDecreaseGauge); // ゲージを減らす
	m_pPlayer->SetIsAttack(true);
	std::shared_ptr<PlayerStateAttack> state = std::make_shared<PlayerStateAttack>(m_pPlayer);
	m_nextState = state;

	// 必殺技アニメーションをランダムで決める
	int specialAnimNo = GetRand(1) + 1;
	std::string specialAnimName = AnimName::kSpecialAtk + std::to_string(specialAnimNo);
	state->Init(specialAnimName);
}

void PlayerStateBase::ChangeStateGuard()
{
	m_pPlayer->SetIsInvincible(false);
	m_pPlayer->SetIsGuard(true);
	std::shared_ptr<PlayerStateGuard> state = std::make_shared<PlayerStateGuard>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateAvoid()
{
	// クールタイム中は回避できないようにする
	if (m_pPlayer->GetAvoidCoolTime() > 0) return;

	m_pPlayer->UpdateAvoid();
	Sound::GetInstance().PlaySe(SoundName::kSe_avoid);
	std::shared_ptr<PlayerStateAvoid> state = std::make_shared<PlayerStateAvoid>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateGrab(Weapon& pWeapon)
{
	// 必殺技中は飛ばす
	if (m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk1 || m_pPlayer->GetCurrentAnim() == AnimName::kSpecialAtk2) return;

	// すでに武器を掴んでいる場合
	if (m_pPlayer->GetIsGrabWeapon())
	{
		m_pPlayer->SetIsGrabWeapon(false); // 武器を離す
		m_pPlayer->SetIsPossibleGrabWeapon(true);
		pWeapon.UpdateIsGrab(false);
		return;
	}

	// 武器を掴んでいない場合かつ範囲内に武器がある場合
	if (m_pPlayer->GetIsPossibleGrabWeapon())
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
}

void PlayerStateBase::ChangeStateDamage()
{
	// すでに再生中の場合は飛ばす
	if (EffectManager::GetInstance().GetIsPlaying(EffectName::kAttack)) return;

	EffectManager::GetInstance().Add(EffectName::kAttack, m_pPlayer->GetPos(), kDamageEffectAdjY);

	std::shared_ptr<PlayerStateHitAttack> state = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::ChangeStateDeath()
{
	if (GetKind() == PlayerStateKind::kDeath) return;

	Sound::GetInstance().PlaySe(SoundName::kSe_down);

	std::shared_ptr<PlayerStateDeath> state = std::make_shared<PlayerStateDeath>(m_pPlayer);
	m_nextState = state;
	state->Init();
}

void PlayerStateBase::VibrationPad(int vibrationPower, int vibrationTime)
{
	StartJoypadVibration(DX_INPUT_PAD1, vibrationPower, vibrationTime, -1);
}
