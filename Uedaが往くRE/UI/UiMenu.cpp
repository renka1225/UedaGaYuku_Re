#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UiMenu.h"

namespace
{
	const std::string kBg = "menu_bg";			 // 背景
	const std::string kMoneyBg = "money_bg";	 // 所持金の背景
	const std::string kMoneyText = "money_text"; // 現在の所持金
}

UiMenu::UiMenu(std::shared_ptr<Player> pPlayer) : 
	UiBase(pPlayer)
{
	m_bgHandle = LoadGraph("data/ui/menu_bg.png");
	m_moneyBg = LoadGraph("data/ui/money_bg.png");
}

UiMenu::~UiMenu()
{
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_moneyBg);
}

void UiMenu::Init()
{
}

void UiMenu::Update()
{
}

void UiMenu::Draw()
{
	UiBase::Draw();

	DrawGraph(0, 0, m_bgHandle, true); // 背景表示
	DrawMoney();
}

void UiMenu::DrawMoney()
{
	Vec2 bgDispPos = { LoadCsv::GetInstance().GetUiData(kMoneyBg).LTposX, LoadCsv::GetInstance().GetUiData(kMoneyBg).LTposY };
	DrawGraphF(bgDispPos.x, bgDispPos.y, m_moneyBg, true);

	// 現在の所持金額
	Vec2 dispPos = { LoadCsv::GetInstance().GetUiData(kMoneyText).LTposX, LoadCsv::GetInstance().GetUiData(kMoneyText).LTposY };
	DrawFormatStringF(dispPos.x, dispPos.y, Color::kColorW, "%d", m_pPlayer->GetMoney());
}
