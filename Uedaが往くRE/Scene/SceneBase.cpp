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
	const Vec2 kDispMoneyTextPos = { 1600.0f, 45.0f };  // 所持金額表示位置

	const char* kCursorSe = "cursor.mp3";
	const char* kSelectSe = "select.mp3";
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

		Sound::GetInstance().PlaySe(kCursorSe);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
		m_pUi->Init();

		Sound::GetInstance().PlaySe(kCursorSe);
	}
}

void SceneBase::DrawMoney(std::shared_ptr<Player> pPlayer)
{
	// 現在の所持金額
	DrawFormatStringFToHandle(kDispMoneyTextPos.x, kDispMoneyTextPos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kMoney)], "%d 円", pPlayer->GetMoney());
}