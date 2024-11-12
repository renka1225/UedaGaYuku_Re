#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "UiBase.h"
#include "Player.h"
#include "SceneEnhance.h"

namespace
{
	// 選択項目
	enum Select
	{
		kHpUp,		// 最大HPアップ
		kGaugeUp,	// 最大ゲージ量アップ
		kAtkUp,		// 攻撃力アップ
		kSelectNum	// 選択数
	};

	// 選択項目のID
	const std::string kId[Select::kSelectNum]
	{
		"hpUp_",	// 最大HPアップID
		"gaugeUp_",	// 最大ゲージ量アップID
		"atkUp_",	// 攻撃力アップID
	};

	constexpr int kMaxStep = 3;	// 最大強化数

	std::string kCursorId = "cursor_enhance";	// カーソルのID
	constexpr float kCursorInterval = 136.0f;	// カーソルの表示間隔
}

SceneEnhance::SceneEnhance(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
	m_select = Select::kHpUp;
	LoadCsv::GetInstance().LoadEnhanceData(m_enhanceData);
}

std::shared_ptr<SceneBase> SceneEnhance::Update(Input& input)
{
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	// Bボタンが押された場合
	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // メニュー画面に戻る
	}

	// Aボタンが押された場合
	if (input.IsTriggered(InputId::kA))
	{
		SetEnhance(); // 選択中のスキルを強化する
	}
	
	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneEnhance::Draw()
{
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval); // カーソルの表示
	DrawMoney(m_pPlayer); // 所持金額表示

	// スキル名を表示する
	DrawSelectSkillName();

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_ENHANCE");

	DrawFormatString(0, 40, 0xffffff, "選択状態:%d", m_select);
#endif
}

void SceneEnhance::SetEnhance()
{
	// 現在の強化段階を取得
	int nowStep = GetSelectSkillStep();
	std::string id = kId[m_select] + std::to_string(nowStep + 1);

	// 最大HPアップ
	if (m_select == Select::kHpUp)
	{
		// 最終段階まで強化されていない場合
		if (nowStep < kMaxStep)
		{
			// プレイヤーに強化状態を適用する
			m_pPlayer->EnhanceHpUp(m_enhanceData[id].upAmount);
		}
	}
	// 最大ゲージ量アップ
	else if (m_select == Select::kGaugeUp)
	{
		// 最終段階まで強化されていない場合
		if (nowStep < kMaxStep)
		{
			// プレイヤーに強化状態を適用する
			m_pPlayer->EnhanceGauge(m_enhanceData[id].upAmount);
		}
	}
	// 攻撃力アップ
	else if (m_select == Select::kAtkUp)
	{
		// 最終段階まで強化されていない場合
		if (nowStep < kMaxStep)
		{
			// プレイヤーに強化状態を適用する
			m_pPlayer->EnhanceAtkUp(m_enhanceData[id].upAmount);
		}
	}
}

void SceneEnhance::DrawSelectSkillName()
{
	int nowStep = GetSelectSkillStep(); // 強化段階取得
	std::string skillId;				// スキルID取得
	std::string skillName;				// スキル名取得

	if (nowStep >= kMaxStep - 1)
	{
		nowStep = kMaxStep - 1;
	}

	skillId = kId[m_select] + std::to_string(nowStep + 1);
	skillName = m_enhanceData[skillId].skillName;

	DrawFormatString(500, 400, Color::kColorW, "スキル名:%s", skillName.c_str());
}

int SceneEnhance::GetSelectSkillStep()
{
	if (m_select == Select::kHpUp)
	{
		return m_pPlayer->GetEnhanceStep().nowHpUpStep;
	}
	else if (m_select == Select::kGaugeUp)
	{
		return m_pPlayer->GetEnhanceStep().nowGaugeUpStep;
	}
	else if (m_select == Select::kAtkUp)
	{
		return m_pPlayer->GetEnhanceStep().nowAtkUpStep;
	}
}
