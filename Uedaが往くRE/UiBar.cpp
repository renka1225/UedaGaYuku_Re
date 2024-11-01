#include "DxLib.h"
#include "LoadCsv.h"
#include "UiBar.h"

namespace
{
	const std::string kBarPlayerHpBack = "bar_playerHpBack";
	const std::string kBarPlayerHp = "bar_playerHp";
	const std::string kBarPlayerGaugeBack = "bar_playerGaugeBack";
	const std::string kBarPlayerGauge = "bar_playerGauge";
	const std::string kBarEnemyHpBack = "bar_enemyHpBack";
	const std::string kBarEnemyHp = "bar_enemyHp";

	constexpr int kPlayerHpColor = 0xee8601;	// プレイヤーのHPの色
	constexpr int kPlayerGaugeColor = 0x1495e8; // プレイヤーのゲージの色
	constexpr int kEnemyHpColor = 0xc60000;		// 敵のゲージの色
}

UiBar::UiBar():
	UiBase(nullptr)
{
	m_barHandle = LoadGraph("data/ui/bar_back.png");
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
}

void UiBar::DrawPlayerHpBar()
{
	// バック部分
	Vec2 dispHpBackLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).LTposY };
	Vec2 dispHpBackRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHpBack).RBposY };
	DrawExtendGraphF(dispHpBackLTPos.x, dispHpBackLTPos.y, dispHpBackRBPos.x, dispHpBackRBPos.y, m_barHandle, true);

	// 現在のHP
	Vec2 dispHpLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposY };
	Vec2 dispHpRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposY };
	DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpRBPos.x, dispHpRBPos.y, kPlayerHpColor, true);
}

void UiBar::DrawPlayerGaugeBar()
{
	// バック部分
	Vec2 dispGaugeBackLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).LTposY };
	Vec2 dispGaugeBackRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeBack).RBposY };
	DrawExtendGraphF(dispGaugeBackLTPos.x, dispGaugeBackLTPos.y, dispGaugeBackRBPos.x, dispGaugeBackRBPos.y, m_barHandle, true);

	// プレイヤーのゲージ
	Vec2 dispGaugeLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).LTposY };
	Vec2 dispGaugeRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerGauge).RBposY };
	DrawBoxAA(dispGaugeLTPos.x, dispGaugeLTPos.y, dispGaugeRBPos.x, dispGaugeRBPos.y, kPlayerGaugeColor, true);
}
