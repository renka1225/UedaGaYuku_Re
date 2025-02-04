#include "Input.h"
#include "Sound.h"
#include "Weapon.h"
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

namespace
{
    constexpr float kSpecialRange = 40.0f;	  // 必殺技が当たる範囲
    constexpr float kSpecialAtkPower = 7.0f;  // 必殺技の攻撃力追加割合
    constexpr int kMinSpecialGauge = 3;       // 攻撃時に溜まるゲージ最小量
    constexpr int kMaxSpecialGauge = 10;      // 攻撃時に溜まるゲージ最大量

    /*コントローラー*/
    constexpr int kVibrationPower = 1000;		// 振動の強さ
    constexpr int kVibrationTime = 150;			// 振動させる時間
    constexpr int kSpecialVibrationPower = 150;	// 必殺技時の振動の強さ
    constexpr int kSpecialVibrationTime = 10;	// 必殺技時の振動させる時間
}

PlayerStateAttack::PlayerStateAttack(const std::shared_ptr<Player>& pPlayer):
    PlayerStateBase(pPlayer),
    m_isAttackEnd(false)
{
}

void PlayerStateAttack::Init(std::string attackName)
{
    m_attackKind = attackName;
	m_pPlayer->ChangeAnim(m_attackKind);

    // 現在のアニメーション終了時間を取得する
    m_animEndTime = GetAnimEndTime();
}

void PlayerStateAttack::Update(const Input& input, const Camera& camera, Stage& stage, Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    PlayerStateBase::Update(input, camera, stage, weapon, pEnemy);
    m_pPlayer->Move(VGet(0.0f, 0.0f, 0.0f), stage);   // 移動情報を反映する

	// 攻撃終了した場合
    if (m_isAttackEnd)
    {
        weapon.SetIsHitAttack(false);
        for (auto& enemy : pEnemy)
        {
            if (enemy == nullptr) continue;
            // 敵の無敵状態を解除する
            enemy->SetIsInvincible(false);
        }

        // StateをIdleに変更する
        ChangeStateIdle();
        m_pPlayer->SetIsAttack(false);
    }
    else
    {
        UpdateAttack(weapon, pEnemy);
    }
}

void PlayerStateAttack::UpdateAttack(Weapon& weapon, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
    for (auto& enemy : pEnemy)
    {
        // 特定の状態の場合はスキップする
        bool isSkip = enemy == nullptr || enemy->GetIsInvincible();
        if (isSkip) continue;

        // 必殺技発動中
        if (m_attackKind == AnimName::kSpecialAtk1 || m_attackKind == AnimName::kSpecialAtk2)
        {
            Sound::GetInstance().PlaySe(SoundName::kSe_specialAttack);

            enemy->SetIsPossibleMove(false); // 敵が動かないようにする

            // プレイヤーと敵の範囲を求める
            float range = VSize(VSub(m_pPlayer->GetPos(), enemy->GetPos()));

            // 範囲内の敵全員にダメージ
            if (range > kSpecialRange) break;

            VibrationPad(kVibrationPower, kVibrationTime); // パッド振動

            enemy->OnDamage(GetAttackPower());
            enemy->SetIsInvincible(true);  // 敵を無敵状態にする
        }

        // 武器攻撃の場合
        if (m_attackKind == AnimName::kOneHandWeapon)
        {
            // 武器と敵の当たり判定を取得
            bool isHitWeaponCol = weapon.CheckWeaponCol(enemy->GetCol(enemy->GetEnemyIndex()), *m_pPlayer);
            if (isHitWeaponCol)
            {
                Sound::GetInstance().PlayBackSe(SoundName::kSe_attack);

                VibrationPad(kVibrationPower, kVibrationTime); // パッド振動

                enemy->OnDamage(GetAttackPower());
                enemy->SetIsInvincible(true);  // 敵を無敵状態にする
                m_pPlayer->UpdateGauge(GetAddGauge());
                weapon.DecrementDurability();
            }
        }

        // パンチ状態かチェック
        bool isPunch = m_attackKind == AnimName::kPunch1 || m_attackKind == AnimName::kPunch2 || m_attackKind == AnimName::kPunch3;
        // パンチ攻撃
        if (isPunch)
        {
            // パンチ攻撃と敵の当たり判定を取得
            bool isHitPunchCol = enemy->CheckHitPunchCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitPunchCol)
            {
                Sound::GetInstance().PlayBackSe(SoundName::kSe_attack);

                VibrationPad(kVibrationPower, kVibrationTime); // パッド振動

                enemy->OnDamage(GetAttackPower());
                enemy->SetIsInvincible(true); // 敵を無敵状態にする
                m_pPlayer->UpdateGauge(GetAddGauge());
             
            }
        }
        // キック攻撃
        else if (m_attackKind == AnimName::kKick)
        {
            // キック攻撃と敵の当たり判定を取得
            bool isHitKickCol = enemy->CheckHitKickCol(m_pPlayer->GetCol(CharacterBase::CharaType::kPlayer), enemy->GetEnemyIndex());
            if (isHitKickCol)
            {
                Sound::GetInstance().PlayBackSe(SoundName::kSe_attack);

                VibrationPad(kVibrationPower, kVibrationTime); // パッド振動

                enemy->OnDamage(GetAttackPower());
                enemy->SetIsInvincible(true); // 敵を無敵状態にする
                m_pPlayer->UpdateGauge(GetAddGauge());
            }
        }
    }

    // アニメーション時間の更新
    m_animEndTime--;
    if (m_animEndTime > 0.0f) return;

    // パンチコマンドが入力されている場合
    if (m_pPlayer->CheckCommand({ InputId::kPunch, InputId::kPunch }, m_pPlayer->GetInputLog()))
    {
        // パンチに移行
        if (m_attackKind == AnimName::kKick)
        {
            Init(AnimName::kPunch1);
            return;
        }

        // 2コンボ目に移行する
        if (m_attackKind == AnimName::kPunch1)
        {
            Init(AnimName::kPunch2);
            return;
        }
        // 3コンボ目に移行する
        else if (m_attackKind == AnimName::kPunch2)
        {
            Init(AnimName::kPunch3);
            return;
        }
        // 現在最終コンボの場合
        else if (m_attackKind == AnimName::kPunch3)
        {
            // 攻撃を終了する
            m_isAttackEnd = true;
            return;
        }
    }
    // コンボ入力がない場合
    else 
    {
        // 攻撃を終了する
        m_isAttackEnd = true;
    }
}

float PlayerStateAttack::GetAnimEndTime()
{
    if (m_attackKind == AnimName::kPunch1) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch1);
    if (m_attackKind == AnimName::kPunch2) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch2);
    if (m_attackKind == AnimName::kPunch3) return m_pPlayer->GetAnimTotalTime(AnimName::kPunch3);
    if (m_attackKind == AnimName::kKick) return m_pPlayer->GetAnimTotalTime(AnimName::kKick);
    if (m_attackKind == AnimName::kSpecialAtk1) return m_pPlayer->GetAnimTotalTime(AnimName::kSpecialAtk1);
    if (m_attackKind == AnimName::kSpecialAtk2) return m_pPlayer->GetAnimTotalTime(AnimName::kSpecialAtk2);
    if (m_attackKind == AnimName::kOneHandWeapon) return m_pPlayer->GetAnimTotalTime(AnimName::kOneHandWeapon);
    if (m_attackKind == AnimName::kTwoHandWeapon) return m_pPlayer->GetAnimTotalTime(AnimName::kTwoHandWeapon);

    return 0.0f;
}

float PlayerStateAttack::GetAttackPower()
{
    // ステータス取得
    auto status =  m_pPlayer->GetStatus();

    if (m_attackKind == AnimName::kPunch1) return status.atkPowerPunch1;
    if (m_attackKind == AnimName::kPunch2) return status.atkPowerPunch2;
    if (m_attackKind == AnimName::kPunch3) return status.atkPowerPunch3;
    if (m_attackKind == AnimName::kKick)  return status.atkPowerKick;
    if (m_attackKind == AnimName::kSpecialAtk1)  return status.atkPowerKick * kSpecialAtkPower;
    if (m_attackKind == AnimName::kSpecialAtk2)  return status.atkPowerKick * kSpecialAtkPower;
    if (m_attackKind == AnimName::kOneHandWeapon)  return status.atkPowerOneHandWeapon;
    if (m_attackKind == AnimName::kTwoHandWeapon)  return status.atkPowerTwoHandWeapon;

    return 0.0f;
}

float PlayerStateAttack::GetAddGauge()
{
    // ゲージを増やす
    float addGauge = static_cast<float>(GetRand((kMaxSpecialGauge - kMinSpecialGauge)) + kMinSpecialGauge);
    return addGauge;
}

#ifdef _DEBUG
std::string PlayerStateAttack::GetStateName()
{
    if (m_attackKind == AnimName::kPunch1) return "パンチ1中";
    if (m_attackKind == AnimName::kPunch2) return "パンチ2中";
    if (m_attackKind == AnimName::kPunch3) return "パンチ3中";
    if (m_attackKind == AnimName::kKick)  return "キック中";
    if (m_attackKind == AnimName::kSpecialAtk1)  return "必殺技1発動中";
    if (m_attackKind == AnimName::kSpecialAtk2)  return "必殺技2発動中";
    if (m_attackKind == AnimName::kOneHandWeapon)  return "片手武器攻撃中";
    if (m_attackKind == AnimName::kTwoHandWeapon)  return "両手武器攻撃中";

    return "アニメーションなし";
}
#endif
