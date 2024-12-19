#include "Game.h"
#include "Vec2.h"
#include "Input.h"
#include "Font.h"
#include "LoadCsv.h"
#include "UiBase.h"
#include "Player.h"
#include "SceneEnhance.h"

namespace
{
	// 画像の種類
	enum Handle
	{
		kBg,		// 背景
		kCircle,	// 画像
		kNum		// 画像の種類
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/enhance/bg.png",
		"data/ui/enhance/circle.png"
	};

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

	// 円の表示位置
	const Vec2 kCirclePos[4]
	{
		{1263.0f, 339.0f},
		{1263.0f, 505.0f},
		{1263.0f, 672.0f},
		{1230.0f, 307.0f},
	};
	constexpr float kCircleIndex = 115.0f;	 // 円の表示間隔
	constexpr int kCircleRadius = 31;		 // 円の半径
	constexpr int kCirclePosnum = 32;		 // 円を形成する頂点数
	constexpr int kCircleColor = 0xba2020;	 // 円の色
	constexpr int kCircleDispSpeed = 2;		 // 円を表示する速度
	constexpr int kCircleDispAnimTime = 240; // アニメーションの時間
	constexpr int kCircleTime = 50;			 // 円を再度表示するまでの時間
	constexpr int kMaxAlpha = 255;			 // 最大アルファ値
	constexpr int kMinAlpha = 100;			 // 最小アルファ値

	const Vec2 kDispSelectSkillNamePos = { 250.0f, 790.0f };	// 選択中のスキル名表示位置
	const Vec2 kDispSelectSkillExplainPos = { 450.0f, 900.0f };	// 選択中のスキル詳細表示位置
	const Vec2 kDispSkillNamePos = { 460.0f, 310.0f };			// スキル名表示位置
	const Vec2 kDispSkillMoneyPos = { 800.0f, 350.0f };			// 必要金額表示位置
	constexpr float kDispSkillInterval = 167.0f;				// 表示間隔

	std::string kCursorId = "cursor_enhance";	// カーソルのID
	constexpr float kCursorInterval = 167.0f;	// カーソルの表示間隔
}

SceneEnhance::SceneEnhance(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer):
	m_circleBlinkTime(0),
	m_circleAlpha(kMinAlpha)
{
	LoadCsv::GetInstance().LoadEnhanceData(m_enhanceData);
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
	m_select = Select::kHpUp;
	
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneEnhance::~SceneEnhance()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneEnhance::Update(Input& input)
{
	// スキルの円部分の表示を更新
	UpdateSkillCircle();
	
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUi->UpdateCursor(kCursorId);

	// Bボタンが押された場合
	if (input.IsTriggered(InputId::kBack))
	{
		SoundCancelSe();
		return m_pPrevScene; // メニュー画面に戻る
	}

	// Aボタンが押された場合
	if (input.IsTriggered(InputId::kA))
	{
		SoundSelectSe();
		SetEnhance(); // 選択中のスキルを強化する
	}
	
	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneEnhance::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kBg], true); // 背景表示
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval); // カーソルの表示

	DrawMoney(m_pPlayer); // 所持金額表示

	// スキル情報を表示する
	DrawSkillCircle();
	DrawSkillInfo();
	DrawSelectSkill();

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

	// 最大まで強化されている場合、または所持金が足りない場合は飛ばす
	bool isSkip = nowStep >= Select::kSelectNum || m_enhanceData[id].requiredMoney > m_pPlayer->GetMoney();
	if (isSkip) return;

	// 最大HPアップ
	if (m_select == Select::kHpUp)
	{
		m_pPlayer->EnhanceHpUp(m_enhanceData[id].upAmount, m_enhanceData[id].requiredMoney);
	}
	// 最大ゲージ量アップ
	else if (m_select == Select::kGaugeUp)
	{
		m_pPlayer->EnhanceGauge(m_enhanceData[id].upAmount, m_enhanceData[id].requiredMoney);
	}
	// 攻撃力アップ
	else if (m_select == Select::kAtkUp)
	{
		m_pPlayer->EnhanceAtkUp(m_enhanceData[id].upAmount, m_enhanceData[id].requiredMoney);
	}
}

void SceneEnhance::UpdateSkillCircle()
{
	m_circleBlinkTime++;

	// 円のアルファ値を調整する
	m_circleBlinkTime += kCircleDispSpeed;
	m_circleBlinkTime %= kCircleDispAnimTime;

	// MEMO:sin波を使って0～1の範囲にする
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_circleBlinkTime) / kCircleDispAnimTime * DX_PI_F);
	m_circleAlpha = kMinAlpha + static_cast<int>((kMaxAlpha - kMinAlpha) * sinAlpha);
}

void SceneEnhance::DrawSkillCircle()
{
	// 強化数を円で表示する
	DrawGraphF(kCirclePos[Select::kSelectNum].x, kCirclePos[Select::kSelectNum].y, m_handle[Handle::kCircle], true);

	// 強化済みの部分を円で表示する
	for (int i = 0; i < m_pPlayer->GetEnhanceStep().nowHpUpStep; i++)
	{
		DrawCircleAA(kCirclePos[Select::kHpUp].x + kCircleIndex * i, kCirclePos[Select::kHpUp].y, kCircleRadius, kCirclePosnum, kCircleColor);
	}
	for (int i = 0; i < m_pPlayer->GetEnhanceStep().nowGaugeUpStep; i++)
	{
		DrawCircleAA(kCirclePos[Select::kGaugeUp].x + kCircleIndex * i, kCirclePos[Select::kGaugeUp].y, kCircleRadius, kCirclePosnum, kCircleColor);
	}
	for (int i = 0; i < m_pPlayer->GetEnhanceStep().nowAtkUpStep; i++)
	{
		DrawCircleAA(kCirclePos[Select::kAtkUp].x + kCircleIndex * i, kCirclePos[Select::kAtkUp].y, kCircleRadius, kCirclePosnum, kCircleColor);
	}

	// 次の強化段階を取得
	int nextEnhanceStep = 0;
	if (m_select == Select::kHpUp)
	{
		nextEnhanceStep = m_pPlayer->GetEnhanceStep().nowHpUpStep;
	}
	else if (m_select == Select::kGaugeUp)
	{
		nextEnhanceStep = m_pPlayer->GetEnhanceStep().nowGaugeUpStep;
	}
	else if (m_select == Select::kAtkUp)
	{
		nextEnhanceStep = m_pPlayer->GetEnhanceStep().nowAtkUpStep;
	}

	// 最大まで強化されている場合は表示しない
	if (nextEnhanceStep >= Select::kSelectNum) return;

	// 選択中スキルの次の強化部分を点滅させる
	if (m_circleBlinkTime >= kCircleTime)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_circleAlpha);
		DrawCircleAA(kCirclePos[m_select].x + kCircleIndex * nextEnhanceStep, kCirclePos[m_select].y, kCircleRadius, kCirclePosnum, kCircleColor);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SceneEnhance::DrawSkillInfo()
{
	std::vector<int> nowStep;
	nowStep.push_back(m_pPlayer->GetEnhanceStep().nowHpUpStep);
	nowStep.push_back(m_pPlayer->GetEnhanceStep().nowGaugeUpStep);
	nowStep.push_back(m_pPlayer->GetEnhanceStep().nowAtkUpStep);

	for (int i = 0; i < Select::kSelectNum; i++)
	{
		std::string skillId = GetSkillId(i, nowStep[i]); // スキルID取得
		std::string skillName = GetSkillName(skillId);	 // スキル名取得
		int skillMoney = GetSkillRequiredMoney(skillId); // 必要金額取得

		// スキル名表示
		DrawFormatStringFToHandle(kDispSkillNamePos.x, kDispSkillNamePos.y + kDispSkillInterval * i, Color::kColorW,
			Font::m_fontHandle[static_cast<int>(Font::FontId::kEnhance_skillName)], "%s", skillName.c_str());
		// 必要金額表示
		DrawFormatStringFToHandle(kDispSkillMoneyPos.x, kDispSkillMoneyPos.y + kDispSkillInterval * i, Color::kColorW,
			Font::m_fontHandle[static_cast<int>(Font::FontId::kEnhance_skillMoney)], "%d 円", skillMoney);
	}
}

void SceneEnhance::DrawSelectSkill()
{
	int nowStep = GetSelectSkillStep(); // 強化段階取得

	// 最大強化数を超えないようにする
	nowStep = std::min(nowStep, Select::kSelectNum - 1);

	std::string skillId = GetSkillId(m_select, nowStep);	// スキルID取得
	std::string skillName = GetSkillName(skillId);			// スキル名取得
	std::string skillExplain = GetSkillExplain(skillId);	// スキル詳細取得

	// スキル名表示
	DrawFormatStringFToHandle(kDispSelectSkillNamePos.x, kDispSelectSkillNamePos.y, Color::kColorW, 
		Font::m_fontHandle[static_cast<int>(Font::FontId::kEnhance_skillName)], "スキル名:%s", skillName.c_str());
	// スキル詳細表示
	DrawFormatStringFToHandle(kDispSelectSkillExplainPos.x, kDispSelectSkillExplainPos.y, Color::kColorW, 
		Font::m_fontHandle[static_cast<int>(Font::FontId::kEnhance_skillExplain)], "スキル詳細:%s", skillExplain.c_str());
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

	return 0;
}

std::string SceneEnhance::GetSkillId(int select, int nowStep)
{
	return kId[select] + std::to_string(nowStep + 1);
}

std::string SceneEnhance::GetSkillName(std::string skillId)
{
	return m_enhanceData[skillId].skillName;
}

std::string SceneEnhance::GetSkillExplain(std::string skillId)
{
	return m_enhanceData[skillId].skillExplain;
}

int SceneEnhance::GetSkillRequiredMoney(std::string skillId)
{
	return m_enhanceData[skillId].requiredMoney;
}
