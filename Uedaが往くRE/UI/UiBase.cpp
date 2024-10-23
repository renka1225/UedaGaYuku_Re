#include "DxLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UiBase.h"

// 定数
namespace
{
	const char* kMiniMapPath = "data/ui/minimap.png"; // ミニマップ画像位置のパス
	const std::string kMoneyId = "player_money"; // 所持金
}

UiBase::UiBase(std::shared_ptr<Player> pPlayer):
	m_minimapHandle(-1)
{
	m_pPlayer = pPlayer;
	LoadCsv::GetInstance().LoadUiData(m_uiData);
	m_minimapHandle = LoadGraph(kMiniMapPath);
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
	// ミニマップ表示
	//DrawRectRotaGraph(30.0f, 800.0f, 0.0f, 0.0f, 1015, 1005, 0.3f, 0.0f, m_minimapHandle, true);

	// 現在の所持金額
	DrawFormatStringF(LoadCsv::GetInstance().GetUiData(kMoneyId).posX, LoadCsv::GetInstance().GetUiData(kMoneyId).posY, Color::kColorW, "所持金:%d", m_pPlayer->GetMoney());
}
