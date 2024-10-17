#include "DxLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UiBase.h"

// 定数
namespace
{
	const std::string kMoneyId = "player_money"; // 所持金
}

UiBase::UiBase(std::shared_ptr<Player> pPlayer):
	m_pos({0.0f, 0.0f})
{
	m_pPlayer = pPlayer;

	LoadCsv::GetInstance().LoadUiData(m_uiData);
}

UiBase::~UiBase()
{
}

void UiBase::Init()
{
}

void UiBase::Update()
{
}

void UiBase::Draw()
{
	// 現在の所持金額
	DrawFormatStringF(LoadCsv::GetInstance().GetUiData(kMoneyId).posX, LoadCsv::GetInstance().GetUiData(kMoneyId).posY, Color::kColorW, "所持金:%d", m_pPlayer->GetMoney());
}
