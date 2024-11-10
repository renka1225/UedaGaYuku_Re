#include "DxLib.h"
#include "LoadCsv.h"
#include "EnemyBase.h"
#include "UiBar.h"

namespace
{
	const std::string kBarHandlePath = "data/ui/bar/";

	const std::string kBarPlayerHpBack = "bar_playerHpBack_";
	const std::string kBarPlayerHp = "bar_playerHp_";
	const std::string kBarPlayerGaugeBack = "bar_playerGaugeBack_";
	const std::string kBarPlayerGauge = "bar_playerGauge_";
	const std::string kBarPlayerGaugeCircle = "bar_playerGaugeCircle";
	const std::string kBarPlayerGaugeMax = "bar_playerGaugeMax";
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

	// ハンドル情報
	enum Handle
	{
		kPlayerHpBg,		// プレイヤーのHP背景
		kPlayerHp,			// プレイヤーのHP
		kPlayerHpDamage,	// プレイヤーのダメージHP
		kPlayerGaugeBg,		// プレイヤーのゲージ背景
		kPlayerGauge,		// プレイヤーのゲージ
		kPlayerGaugeCircle, // プレイヤーのゲージ円部分
		kPlayerGaugeMax,	// プレイヤーのゲージが溜まったとき
		kEnemyHpBg,			// 敵のHP背景部分
		kEnemyHp,			// 敵のHP
		kEnemyHpDamage,		// 敵のダメージHP
		kNum,				// ハンドルの種類
	};
}

UiBar::UiBar():
	m_damage(0),
	m_hpDecreaseTime(0)
{
	LoadHandle();
}

UiBar::~UiBar()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void UiBar::Init()
{
}

void UiBar::Update()
{
	m_hpDecreaseTime--;
	m_hpDecreaseTime = std::max(0, m_hpDecreaseTime);
}

void UiBar::LoadHandle()
{
	m_handle.resize(Handle::kNum);
	m_handle[Handle::kPlayerHpBg] = LoadGraph((kBarHandlePath + "player_hp_bg.png").c_str());
	m_handle[Handle::kPlayerHp] = LoadGraph((kBarHandlePath + "player_hp.png").c_str());
	m_handle[Handle::kPlayerHpDamage] = LoadGraph((kBarHandlePath + "player_hp_damage.png").c_str());
	m_handle[Handle::kPlayerGaugeBg] = LoadGraph((kBarHandlePath + "player_gauge_bg.png").c_str());
	m_handle[Handle::kPlayerGauge] = LoadGraph((kBarHandlePath + "player_gauge.png").c_str());
	m_handle[Handle::kPlayerGaugeCircle] = LoadGraph((kBarHandlePath + "player_gauge_circle.png").c_str());
	m_handle[Handle::kPlayerGaugeMax] = LoadGraph((kBarHandlePath + "player_gauge_max.png").c_str());
	m_handle[Handle::kEnemyHpBg] = LoadGraph((kBarHandlePath + "enemy_hp_bg.png").c_str());
	m_handle[Handle::kEnemyHp] = LoadGraph((kBarHandlePath + "enemy_hp.png").c_str());
	m_handle[Handle::kEnemyHpDamage] = LoadGraph((kBarHandlePath + "enemy_hp_damage.png").c_str());
}

void UiBar::DrawPlayerHpBar(float currentHp, float maxHp)
{
	// TODO:現在数字を直接書いてる所は現在の強化量

	// バック部分
	std::string bgId = kBarPlayerHpBack + "3"; // 最大HPによってIDを変える
	auto bgData = LoadCsv::GetInstance().GetUiData(bgId);
	DrawExtendGraphF(bgData.LTposX, bgData.LTposY, bgData.RBposX, bgData.RBposY, m_handle[Handle::kPlayerHpBg], true);

	// ダメージバー
	std::string damageId = kBarPlayerHp + "3"; // 最大HPによってIDを変える
	auto damageData = LoadCsv::GetInstance().GetUiData(damageId);
	DrawExtendGraphF(damageData.LTposX, damageData.LTposY, damageData.RBposX, damageData.RBposY, m_handle[Handle::kPlayerHpDamage], true);

	// HPバー
	std::string hpId = kBarPlayerHp + "3"; // 最大HPによってIDを変える
	auto hpData = LoadCsv::GetInstance().GetUiData(hpId);
	DrawExtendGraphF(hpData.LTposX, hpData.LTposY, hpData.RBposX, hpData.RBposY, m_handle[Handle::kPlayerHp], true);


	//Vec2 dispHpLTPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).LTposY };
	//Vec2 dispHpRBPos = { LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposX, LoadCsv::GetInstance().GetUiData(kBarPlayerHp).RBposY };
	//float dispHpBarWidth = LoadCsv::GetInstance().GetUiData(kBarPlayerHp).width; // HPバーの長さ

	// 現在のHP量に応じてバーの長さを変える
	//float hpRatio = currentHp / maxHp;
	//float hpLength = dispHpBarWidth * hpRatio;

	// ダメージ部分
	//float decreaseHpRatio = (currentHp + m_damage) / maxHp;
	//float decreaseHpLength = dispHpBarWidth * decreaseHpRatio;

	// ダメージ分ゲージの色を変える
	//if (m_hpDecreaseTime >= 0)
	//{
		//DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + decreaseHpLength, dispHpRBPos.y, kPlayerHpDamageColor, true);
	//}
	// HPバー表示
	//DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + hpLength, dispHpRBPos.y, kPlayerHpColor, true);
}

void UiBar::DrawPlayerGaugeBar(float currentGauge, float maxGauge)
{
	// バック部分
	std::string bgId = kBarPlayerGaugeBack + "3"; // 最大ゲージ量によってIDを変える
	auto bgData = LoadCsv::GetInstance().GetUiData(bgId);
	DrawExtendGraphF(bgData.LTposX, bgData.LTposY, bgData.RBposX, bgData.RBposY, m_handle[Handle::kPlayerGaugeBg], true);

	// ゲージバー
	std::string gaugeId = kBarPlayerGauge + "3"; // 最大ゲージ量によってIDを変える
	auto gaugeData = LoadCsv::GetInstance().GetUiData(gaugeId);
	DrawExtendGraphF(gaugeData.LTposX, gaugeData.LTposY, gaugeData.RBposX, gaugeData.RBposY, m_handle[Handle::kPlayerGauge], true);

	auto gaugeCircleData = LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeCircle);
	DrawExtendGraphF(gaugeCircleData.LTposX, gaugeCircleData.LTposY, gaugeCircleData.RBposX, gaugeCircleData.RBposY, m_handle[Handle::kPlayerGaugeCircle], true);

	if (currentGauge >= maxGauge)
	{
		auto gaugeMaxData = LoadCsv::GetInstance().GetUiData(kBarPlayerGaugeMax);
		DrawExtendGraphF(gaugeMaxData.LTposX, gaugeMaxData.LTposY, gaugeMaxData.RBposX, gaugeMaxData.RBposY, m_handle[Handle::kPlayerGaugeMax], true);
	}

//	// 現在のゲージ量に応じてバーの長さを変える
//	float gaugeRatio = currentGauge / maxGauge;
//	float gaugeLength = dispGaugeBarWidth * gaugeRatio;
//
//	// プレイヤーのゲージを表示
//	DrawBoxAA(dispGaugeLTPos.x, dispGaugeLTPos.y, dispGaugeLTPos.x + gaugeLength, dispGaugeRBPos.y, kPlayerGaugeColor, true);
}

void UiBar::DrawEnemyHpBar(EnemyBase& pEnemy)
{
	// スクリーン座標に変換する
	VECTOR barDispPos = VAdd(pEnemy.GetPos(), VGet(0.0f, kAdjDispBarPosY, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(barDispPos);

	// バック部分
	std::string bgId = kBarEnemyHpBack;
	auto bgData = LoadCsv::GetInstance().GetUiData(bgId);
	DrawExtendGraphF(screenPos.x - bgData.width, screenPos.y - bgData.height,
		screenPos.x + bgData.width, screenPos.y + bgData.height, m_handle[Handle::kEnemyHpBg], true);

	// ダメージバー
	std::string damageId = kBarEnemyHp;
	auto damageData = LoadCsv::GetInstance().GetUiData(damageId);

	DrawExtendGraphF(screenPos.x - damageData.width, screenPos.x - damageData.height,
		screenPos.x + damageData.width, screenPos.x + damageData.height, m_handle[Handle::kEnemyHpDamage], true);

	// HPバー
	std::string hpId = kBarEnemyHp;
	auto hpData = LoadCsv::GetInstance().GetUiData(hpId);
	DrawExtendGraphF(screenPos.x - hpData.width, screenPos.x - hpData.height,
		screenPos.x + hpData.width, screenPos.x + hpData.height, m_handle[Handle::kEnemyHp], true);


	//// ゲージの最大の長さ
	//Vec2 dispHpLTPos = { screenPos.x - kBarEnemyHpSize.x, screenPos.y - kBarEnemyHpSize.y };
	//Vec2 dispHpRBPos = { screenPos.x + kBarEnemyHpSize.x, screenPos.y + kBarEnemyHpSize.y };
	//float dispHpBarWidth = LoadCsv::GetInstance().GetUiData(kBarEnemyHp).width; // ゲージバーの長さ

	//// 現在のゲージ量に応じてバーの長さを変える
	//float hpRatio = pEnemy.GetHp() / pEnemy.GetStatus().maxHp;
	//float hpLength = dispHpBarWidth * hpRatio;

	//// ダメージ部分
	//float decreaseHpRatio = (pEnemy.GetHp() + m_damage) / pEnemy.GetStatus().maxHp;
	//float decreaseHpLength = dispHpBarWidth * decreaseHpRatio;

	//// ダメージ分ゲージの色を変える
	//if (m_hpDecreaseTime >= 0)
	//{
	//	DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + decreaseHpLength, dispHpRBPos.y, kEnemyHpDamageColor, true);
	//}

	//// 敵のHPを表示
	//DrawBoxAA(dispHpLTPos.x, dispHpLTPos.y, dispHpLTPos.x + hpLength, dispHpRBPos.y, kEnemyHpColor, true);
}

void UiBar::SetDamageTimer()
{
	if (m_hpDecreaseTime <= 0)
	{
		m_hpDecreaseTime = kIntervalTime;
	}
}

void UiBar::SetDamage(float damage)
{
	m_damage = damage;
	SetDamageTimer();
}
