#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Sound.h"
#include "Font.h"
#include "UiBase.h"
#include "Item.h"
#include "Player.h"
#include "SceneBase.h"

// 定数
namespace
{
	const Vec2 kCurrentScenePos = { 0.0f, 0.0f };		// 現在のシーン名表示位置
	const Vec2 kDispMoneyBgPos = { 1320.0f, 15.0f };	// 金額の背景表示位置
	const Vec2 kDispMoneyTextPos = { 1800.0f, 45.0f };  // 所持金額表示位置
}

SceneBase::SceneBase():
	m_select(0)
{
	m_pItem = std::make_shared<Item>();
	m_pUi = std::make_shared<UiBase>();
}

SceneBase::~SceneBase()
{
}

void SceneBase::EndGame()
{
	Sound::GetInstance().UnLoad(); // サウンドの解放
	Font::UnLoad();				   // フォントの解放
	Effkseer_End();				   // Effekseerの終了処理
	DxLib_End();				   // Dxライブラリ使用の終了処理
}

void SceneBase::SoundSelectSe()
{
	Sound::GetInstance().PlaySe(SoundName::kSe_select);
}

#ifdef _DEBUG
void SceneBase::DrawSceneText(const char* sceneName)
{
	DrawStringF(kCurrentScenePos.x, kCurrentScenePos.y, LoadCsv::GetInstance().Get_cMessage(sceneName), Color::kColorW);
}
#endif

void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered(InputId::kDown))
	{
		m_select = (m_select + 1) % selectNum;
		m_pUi->Init();

		Sound::GetInstance().PlaySe(SoundName::kSe_cursor);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
		m_pUi->Init();

		Sound::GetInstance().PlaySe(SoundName::kSe_cursor);
	}
}

void SceneBase::DrawMoney(std::shared_ptr<Player> pPlayer)
{
	// 現在の所持金額を取得
	int money = pPlayer->GetMoney();
	int man = money / 10000; // 万単位
	int yen = money % 10000; // 円単位

	// 表示用のテキスト
	std::string moneyText;

	// 万の表示
	if (man > 0)
	{
		moneyText += std::to_string(man) + "万";

		// 円の表示
		if (yen > 0)
		{
			moneyText += std::to_string(yen) + "円";
		}
	}
	// 円の表示のみ
	else
	{
		moneyText = std::to_string(yen) + "円";
	}

	// 金額部分を右詰めにする
	int moneyWidth = GetDrawStringWidthToHandle(moneyText.c_str(), moneyText.size(), Font::m_fontHandle[static_cast<int>(Font::FontId::kMoney)]);
	float dispX = kDispMoneyTextPos.x - moneyWidth;

	// 現在の所持金額
	DrawStringFToHandle(dispX, kDispMoneyTextPos.y, moneyText.c_str(), Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kMoney)]);
}