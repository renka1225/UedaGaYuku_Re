#include "DxLib.h"
#include "LoadCsv.h"
#include "EnemyBase.h"
#include "UiBar.h"

namespace
{
	const char* kBarHandlePath = "data/ui/bar_back.png";

	const std::string kBarPlayerHpBack = "bar_playerHpBack";
	const std::string kBarPlayerHp = "bar_playerHp";
	const std::string kBarPlayerGaugeBack = "bar_playerGaugeBack";
	const std::string kBarPlayerGauge = "bar_playerGauge";
	const std::string kBarEnemyHpBack = "bar_enemyHpBack";
	const std::string kBarEnemyHp = "bar_enemyHp";

	constexpr float kAdjDispBarPosY = 30.0f;			// 敵HPバーの表示位置調整
	const Vec2 kBackBarEnemyHpSize = { 80.0f, 10.0f };	// 敵HPバーのバック部分のサイズ
	const Vec2 kBarEnemyHpSize = { 66.0f, 5.0f };		// 敵HPバーのサイズ

	constexpr int kPlayerHpColor = 0xee8601;		// プレイヤーのHPの色
	constexpr int kPlayerHpDamageColor = 0xc60000;	// プレイヤーHPのダメージ色
	constexpr int kPlayerGaugeColor = 0x1495e8;		// プレイヤーのゲージの色
	constexpr int kEnemyHpColor = 0xc60000;			// 敵のゲージの色
	constexpr int kEnemyHpDamageColor = 0xe97a7a;	// プレイヤーのゲージの色

	constexpr int kIntervalTime = 120;				// HPバーが減少するまでの時間
}

UiBar::UiBar():
	m_damage(0),
	m_hpDecreaseTime(0)
{
	m_barHandle = LoadGraph(kBarHandlePath);
}

UiBar::~UiBar()
{
	DeleteGraph(m_barHandle);
}

void UiBar::Init()
{
}

void UiBar::Update()
{
	m_hpDecreaseTime--;
	m_hpDecreaseTime = std::max(0, m_hpDecreaseTime);
}

void UiBar::DrawPlayerHpBar(float currentHp, float maxHp)
{
	// バック部分
	Vec2 dispHpBackLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).LTposY };
	Vec2 dispHpBackRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).RBposY };
	DrawExtendGraphF(dispHpBackLTPos.x, dispHpBackLTPos.y, dispHpBackRBPos.x, dispHpBackRBPos.y, m_barHandle, true);

	// HPの最大の長さ
	Vec2 dispHpLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposY };
	Vec2 dispHpRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposY };
	float dispHpBarWidth = LoadCsv::GetInstance().GetUiData(kBarPlayerHp).width; // HPバーの長さ

	// 現在のHP量に応じてバーの長さを変える
	float hpRatio = currentHp / maxHp;
	float hpLength = dispHpBarWidth * hpRatio;

	// ダメージ部分
	float decreaseHpRatio = (currentHp + m_damage) / maxHp;
	float decreaseHpLength = dispHpBarWidth * decreaseHpRatio;

	// ダメージ分ゲージの色を変える
	if (m_hpDecreaseTime > 0)
	{
		DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + decreaseHpLength, dispHpRBPos.y, kPlayerHpDamageColor, true);
	}
	// HPバー表示
	DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + hpLength, dispHpRBPos.y, kPlayerHpColor, true);
}

void UiBar::DrawPlayerGaugeBar(float currentGauge, float maxGauge)
{
	// バック部分
	Vec2 dispGaugeBackLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).LTposY };
	Vec2 dispGaugeBackRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).RBposY };
	DrawExtendGraphF(dispGaugeBackLTPos.x, dispGaugeBackLTPos.y, dispGaugeBackRBPos.x, dispGaugeBackRBPos.y, m_barHandle, true);

	// ゲージの最大の長さ
	Vec2 dispGaugeLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).LTposY };
	Vec2 dispGaugeRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).RBposY };
	float dispGaugeBarWidth = LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).width; // ゲージバーの長さ

	// 現在のゲージ量に応じてバーの長さを変える
	float gaugeRatio = currentGauge / maxGauge;
	float gaugeLength = dispGaugeBarWidth * gaugeRatio;

	// プレイヤーのゲージを表示
	DrawBoxAA(dispGaugeLTPos.x, dispGaugeLTPos.y, dispGaugeLTPos.x + gaugeLength, dispGaugeRBPos.y, kPlayerGaugeColor, true);
}

void UiBar::DrawEnemyHpBar(EnemyBase& pEnemy)
{
	// スクリーン座標に変換する
	VECTOR barDispPos = VAdd(pEnemy.GetPos(), VGet(0.0f, kAdjDispBarPosY, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(barDispPos);

	// バック部分
	DrawExtendGraphF(screenPos.x - kBackBarEnemyHpSize.x, screenPos.y - kBackBarEnemyHpSize.y,
		screenPos.x + kBackBarEnemyHpSize.x, screenPos.y + kBackBarEnemyHpSize.y, m_barHandle, true);

	// ゲージの最大の長さ
	Vec2 dispHpLTPos = { screenPos.x - kBarEnemyHpSize.x, screenPos.y - kBarEnemyHpSize.y };
	Vec2 dispHpRBPos = { screenPos.x + kBarEnemyHpSize.x, screenPos.y + kBarEnemyHpSize.y };
	float dispHpBarWidth = LoadCsv::GetInstance().GetUiData(kBarEnemyHp).width; // ゲージバーの長さ

	// 現在のゲージ量に応じてバーの長さを変える
	float hpRatio = pEnemy.GetHp() / pEnemy.GetStatus().maxHp;
	float hpLength = dispHpBarWidth * hpRatio;

	// ダメージ部分
	float decreaseHpRatio = (pEnemy.GetHp() + m_damage) / pEnemy.GetStatus().maxHp;
	float decreaseHpLength = dispHpBarWidth * decreaseHpRatio;

	// ダメージ分ゲージの色を変える
	if (m_hpDecreaseTime > 0)
	{
		DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + decreaseHpLength, dispHpRBPos.y, kEnemyHpDamageColor, true);
	}

	// 敵のHPを表示
	DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + hpLength, dispHpRBPos.y, kEnemyHpColor, true);
}

void UiBar::SetDamageTimer()
{
	if (m_hpDecreaseTime < 0)
	{
		m_hpDecreaseTime = kIntervalTime;
	}
}

void UiBar::SetDamage(float damage)
{
	m_damage = damage;
	SetDamageTimer();
}
