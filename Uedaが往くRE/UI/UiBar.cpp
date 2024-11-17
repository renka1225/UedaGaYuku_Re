#include "DxLib.h"
#include "LoadCsv.h"
#include "Player.h"
#include "EnemyBase.h"
#include "UiBar.h"
#include <cassert>

namespace
{
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

	// 画像のファイル名
	const std::string kBarHandleFile = "data/ui/bar/";
	const std::string kBarHandleName[Handle::kNum]
	{
		kBarHandleFile + "player_hp_bg.png",
		kBarHandleFile + "player_hp.png",
		kBarHandleFile + "player_hp_damage.png",
		kBarHandleFile + "player_gauge_bg.png",
		kBarHandleFile + "player_gauge.png",
		kBarHandleFile + "player_gauge_circle.png",
		kBarHandleFile + "player_gauge_max.png",
		kBarHandleFile + "enemy_hp_bg.png",
		kBarHandleFile + "enemy_hp.png",
		kBarHandleFile + "enemy_hp_damage.png",
	};

	// バーのID
	const std::map<std::string, std::string> kBarID
	{
		{"playerHpBack", "bar_playerHpBack_"},
		{"playerHp", "bar_playerHp_"},
		{"playerGaugeBack", "bar_playerGaugeBack_"},
		{"playerGauge", "bar_playerGauge_"},
		{"playerGaugeCircle", "bar_playerGaugeCircle"},
		{"playerGaugeMax", "bar_playerGaugeMax"},
		{"enemyHpBack", "bar_enemyHpBack"},
		{"enemyHp", "bar_enemyHp"},
	};

	constexpr float kAdjDispBarPosY = 30.0f;			// 敵HPバーの表示位置調整
	const Vec2 kBackBarEnemyHpSize = { 80.0f, 10.0f };	// 敵HPバーのバック部分のサイズ
	const Vec2 kBarEnemyHpSize = { 66.0f, 5.0f };		// 敵HPバーのサイズ

	constexpr int kIntervalTime = 180;	// HPバーが減少するまでの時間
}

UiBar::UiBar():
	m_playerDamage(0),
	m_enemyDamage(0),
	m_playerHpDecreaseTime(0),
	m_enemyHpDecreaseTime(0)
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
	if (m_playerHpDecreaseTime > 0)
	{
		m_playerHpDecreaseTime--;
	}
	// 一定時間経ったら、ダメージ分のHPバーを徐々に減らす
	else if (m_playerDamage > 0 && m_playerHpDecreaseTime <= 0)
	{
		m_playerDamage--;
		m_playerDamage = std::max(0.0f, m_playerDamage);
	}

	if (m_enemyHpDecreaseTime > 0)
	{
		m_enemyHpDecreaseTime--;
	}
	else if (m_enemyDamage > 0 && m_enemyHpDecreaseTime <= 0)
	{
		m_enemyDamage--;
		m_enemyDamage = std::max(0.0f, m_enemyDamage);
	}
}

void UiBar::LoadHandle()
{
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kBarHandleName[i].c_str());
		assert(m_handle[i] != -1);
	}
}

void UiBar::DrawPlayerHpBar(Player& player, float maxHp)
{
	// 現在の強化段階を取得
	std::string enhanceStep = std::to_string(player.GetEnhanceStep().nowHpUpStep);

	/*バック部分*/
	std::string bgId = kBarID.at("playerHpBack") + enhanceStep; // 最大HPによってIDを変える
	auto bgData = LoadCsv::GetInstance().GetUiData(bgId);
	DrawExtendGraphF(bgData.LTposX, bgData.LTposY, bgData.RBposX, bgData.RBposY, m_handle[Handle::kPlayerHpBg], true);

	/*ダメージバー*/
	std::string damageId = kBarID.at("playerHp") + enhanceStep; // 最大HPによってIDを変える
	auto damageData = LoadCsv::GetInstance().GetUiData(damageId);

	// ダメージバーの長さを変える
	float damageHpRatio = (player.GetHp() + m_playerDamage) / maxHp;
	float damageHpLength = damageData.RBposX * damageHpRatio;
	DrawExtendGraphF(damageData.LTposX, damageData.LTposY, damageHpLength, damageData.RBposY, m_handle[Handle::kPlayerHpDamage], true);

	/*HPバー*/
	std::string hpId = kBarID.at("playerHp") + enhanceStep; // 最大HPによってIDを変える
	auto hpData = LoadCsv::GetInstance().GetUiData(hpId);

	// 現在のHP量に応じてバーの長さを変える
	float hpRatio = player.GetHp() / maxHp;
	float hpLength = hpData.RBposX * hpRatio;
	DrawExtendGraphF(hpData.LTposX, hpData.LTposY, hpLength, hpData.RBposY, m_handle[Handle::kPlayerHp], true);
}

void UiBar::DrawPlayerGaugeBar(Player& player, float maxGauge)
{
	// 現在の強化段階を取得
	std::string enhanceStep = std::to_string(player.GetEnhanceStep().nowGaugeUpStep);

	// バック部分
	std::string bgId = kBarID.at("playerGaugeBack") + enhanceStep; // 最大ゲージ量によってIDを変える
	auto bgData = LoadCsv::GetInstance().GetUiData(bgId);
	DrawExtendGraphF(bgData.LTposX, bgData.LTposY, bgData.RBposX, bgData.RBposY, m_handle[Handle::kPlayerGaugeBg], true);

	// ゲージバー
	std::string gaugeId = kBarID.at("playerGauge") + enhanceStep; // 最大ゲージ量によってIDを変える
	auto gaugeData = LoadCsv::GetInstance().GetUiData(gaugeId);

	// 現在のゲージ量に応じてバーの長さを変える
	float gaugeRatio = player.GetGauge() / maxGauge;
	float gaugeLength = gaugeData.RBposX * gaugeRatio;
	DrawExtendGraphF(gaugeData.LTposX, gaugeData.LTposY, gaugeData.LTposX + gaugeLength, gaugeData.RBposY, m_handle[Handle::kPlayerGauge], true);

	// ゲージの円部分
	auto gaugeCircleData = LoadCsv::GetInstance().GetUiData(kBarID.at("playerGaugeCircle"));
	DrawExtendGraphF(gaugeCircleData.LTposX, gaugeCircleData.LTposY, gaugeCircleData.RBposX, gaugeCircleData.RBposY, m_handle[Handle::kPlayerGaugeCircle], true);

	// ゲージが最大まで溜まっている場合
	if (player.GetGauge() >= maxGauge)
	{
		// 円の色を変える
		auto gaugeMaxData = LoadCsv::GetInstance().GetUiData(kBarID.at("playerGaugeMax"));
		DrawExtendGraphF(gaugeMaxData.LTposX, gaugeMaxData.LTposY, gaugeMaxData.RBposX, gaugeMaxData.RBposY, m_handle[Handle::kPlayerGaugeMax], true);
	}
}

void UiBar::DrawEnemyHpBar(EnemyBase& pEnemy)
{
	// スクリーン座標に変換する
	VECTOR barDispPos = VAdd(pEnemy.GetPos(), VGet(0.0f, kAdjDispBarPosY, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(barDispPos);

	// バック部分
	auto bgData = LoadCsv::GetInstance().GetUiData(kBarID.at("enemyHpBack"));
	DrawExtendGraphF(screenPos.x - bgData.width, screenPos.y - bgData.height,
		screenPos.x + bgData.width, screenPos.y + bgData.height, m_handle[Handle::kEnemyHpBg], true);

	// ダメージバー
	auto damageData = LoadCsv::GetInstance().GetUiData(kBarID.at("enemyHp"));

	// ダメージバーの長さを変える
	float damageHpRatio = (pEnemy.GetHp() + m_enemyDamage) / pEnemy.GetStatus().maxHp;
	float damageHpLength = damageData.width * 2 * damageHpRatio;

	DrawExtendGraphF(screenPos.x - damageData.width, screenPos.y - damageData.height,
		(screenPos.x - damageData.width) + damageHpLength, screenPos.y + damageData.height, m_handle[Handle::kEnemyHpDamage], true);

	// HPバー
	auto hpData = LoadCsv::GetInstance().GetUiData(kBarID.at("enemyHp"));

	// HPバーの長さを変える
	float hpRatio = pEnemy.GetHp() / pEnemy.GetStatus().maxHp;
	float hpLength = hpData.width * 2 * hpRatio;

	DrawExtendGraphF(screenPos.x - hpData.width, screenPos.y - hpData.height,
		(screenPos.x - hpData.width) + hpLength, screenPos.y + hpData.height, m_handle[Handle::kEnemyHp], true);
}

void UiBar::SetPlayerDamage(float damage)
{
	m_playerDamage = damage;
	m_playerHpDecreaseTime = kIntervalTime;
}

void UiBar::SetEnemyDamage(float damage)
{
	m_enemyDamage = damage;
	m_enemyHpDecreaseTime = kIntervalTime;
}