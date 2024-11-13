#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Font.h"
#include "Item.h"
#include "Player.h"
#include "SceneUseItem.h"

namespace
{
	const char* kBgHandlePath = "data/ui/useItem/bg.png";
	const char* kCursorHandlePath = "data/ui/useItem/cursor.png";

	constexpr int kSelectMin = 0;	 // 選択アイテムの最小番号
	constexpr int kSelectMax = 12;	 // 選択アイテムの最大番号
	constexpr int kSelectRow = 2;	 // 選択アイテムの行数
	constexpr int kSelectCol = 6;	 // 選択アイテムの列数
	
	const Vec2 kCursorPos = { 420.0f, 280.0f };			// 初期カーソル表示位置
	const Vec2 kCursorMove = { 178.0f, 220.0f };		// カーソルの移動量
	const Vec2 kDispItemNamePos = { 250.0f, 790.0f };	// アイテム名表示位置
	const Vec2 kDispItemExplainPos = { 300.0f, 900.0f };// アイテム説明表示位置

	enum Handle
	{
		kBg,	 // 背景
		kText,	 // テキスト
		kItem,	 // アイテム
		kCursor, // カーソル
		kNum
	};
}

SceneUseItem::SceneUseItem(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
	m_pItem = std::make_shared<Item>();

	m_select = kSelectMin;
	m_possessItem.resize(kSelectMax);

	m_handle.resize(Handle::kNum);
	m_handle[Handle::kBg] = LoadGraph(kBgHandlePath);
	m_handle[Handle::kCursor] = LoadGraph(kCursorHandlePath);
}

SceneUseItem::~SceneUseItem()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void SceneUseItem::Init()
{
}

std::shared_ptr<SceneBase> SceneUseItem::Update(Input& input)
{
	// プレイヤーの所持しているアイテム情報を取得する
	m_possessItem = m_pPlayer->GetPossessItem();

	MoveCursor(input); 	// カーソル移動の処理

	// 決定ボタンを押したらアイテムを使用する
	if (input.IsTriggered(InputId::kA))
	{
		UseItem();
	}

	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // メニュー画面に戻る
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneUseItem::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kBg], true);
	DrawCursor(); // カーソル表示

	DrawMoney(m_pPlayer); // 所持金額表示

	// アイテムの情報を表示する
	for (int i = 0; i < m_possessItem.size(); i++)
	{
		// アイテムの情報がない部分は無視する
		if (m_possessItem[i] == -1) continue;
		
		// アイテムの画像を表示する
		//DrawGraph(100 + 200 * i, 300, m_handle[i], true);

		// 選択中のアイテム名を表示する
		if (m_select == i)
		{
			std::string itemName = m_pItem->GetItemData(m_possessItem[i]).itemName;
			std::string itemExplain = m_pItem->GetItemData(m_possessItem[i]).itemExplain;

			DrawFormatStringFToHandle(kDispItemNamePos.x, kDispItemNamePos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kMenu_itemName)],
				"%s", itemName.c_str());

			DrawFormatStringFToHandle(kDispItemExplainPos.x, kDispItemExplainPos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kMenu_itemExplain)],
				"%s", itemExplain.c_str());
		}
	}

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_USEITEM");
#endif
}

void SceneUseItem::UseItem()
{
	// 現在選択中のアイテムを使用する
	m_pItem->ApplyEffect(*m_pPlayer, m_possessItem[m_select]);

	// 使用したアイテムを削除する
	m_pPlayer->DeleteItem(m_select);

#ifdef _DEBUG
	printfDx("%dのアイテム使用した\n", m_possessItem[m_select]);
#endif

}

void SceneUseItem::MoveCursor(Input& input)
{
	// 右に入力
	if (input.IsTriggered(InputId::kRight))
	{
		// カーソルが一番右に来たら一番左に移動させる
		if (m_select % kSelectCol >= (kSelectCol - 1))
		{
			m_select -= kSelectCol;
		}

		m_select++;
	}
	// 左に入力
	else if (input.IsTriggered(InputId::kLeft))
	{
		// カーソルが1番左に来たら一番右に移動させる
		if (m_select % kSelectCol == 0)
		{
			m_select += (kSelectCol - 1);
		}
		else
		{
			m_select--;
		}
	}
	// 上に入力
	else if (input.IsTriggered(InputId::kUp))
	{
		m_select -= kSelectCol;
		// カーソルが1番上に来たら一番下に移動させる
		if (m_select < 0)
		{
			m_select += kSelectCol * 2;
		}
	}
	// 下に入力
	else if (input.IsTriggered(InputId::kDown))
	{
		m_select += kSelectCol;
		// カーソルが1番下に来たら一番上に移動させる
		if (m_select >= kSelectMax)
		{
			m_select -= kSelectCol * 2;
		}
	}
}

void SceneUseItem::DrawCursor()
{
	float dispY;
	if (m_select < kSelectCol)
	{
		dispY = kCursorPos.y;
	}
	else
	{
		dispY = kCursorPos.y + kCursorMove.y;
	}

	DrawGraphF(kCursorPos.x + ((m_select % kSelectCol) * kCursorMove.x), dispY, m_handle[Handle::kCursor], true);
}
