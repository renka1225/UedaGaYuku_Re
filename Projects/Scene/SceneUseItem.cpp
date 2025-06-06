﻿#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Sound.h"
#include "Input.h"
#include "Font.h"
#include "Item.h"
#include "Player.h"
#include "SceneUseItem.h"

namespace
{
	// 画像の種類
	enum Handle
	{
		kBg,	 // 背景
		kItem,	 // アイテム
		kCursor, // カーソル
		kNum	 // 画像の種類数
	};

	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/useItem/bg.png",
		"data/ui/useItem/item.png",
		"data/ui/useItem/cursor.png"
	};

	// 表示位置
	const std::map<std::string, Vec2> kDispPos
	{
		{"cursor", { 420.0f, 280.0f }},			// 初期カーソル
		{"itemName", { 250.0f, 790.0f }},		// アイテム名
		{"itemExplain", { 300.0f, 900.0f }},	// アイテム説明
		{"itemHandle", {505.0f, 390.0f}}		// アイテム画像
	};
	const Vec2 kItemHandleInterval = { 180.0f, 220.0f };	// アイテム画像表示間隔
	const Vec2 kItemSize = {73.0f, 182.0f};					// アイテム画像の1つあたりのサイズ

	const Vec2 kCursorMove = { 178.0f, 220.0f }; // カーソルの移動量

	constexpr int kSelectMin = 0;	 // 選択アイテムの最小番号
	constexpr int kSelectMax = 12;	 // 選択アイテムの最大番号
	constexpr int kSelectRow = 2;	 // 選択アイテムの行数
	constexpr int kSelectCol = 6;	 // 選択アイテムの列数
	constexpr int kDispTime = 3;	// アイテム画像を表示し始める時間
}

SceneUseItem::SceneUseItem(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer):
	m_time(0)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;

	m_select = kSelectMin;
	m_possessItem.resize(kSelectMax);

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneUseItem::~SceneUseItem()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneUseItem::Update(Input& input)
{
	m_time++;

	// プレイヤーの所持しているアイテム情報を取得する
	m_possessItem = m_pPlayer->GetPossessItem();

	MoveCursor(input); 	// カーソル移動の処理

	// Bボタンを押した場合
	if (input.IsTriggered(InputId::kBack))
	{
		SoundCancelSe();
		return m_pPrevScene; // メニュー画面に戻る
	}

	// 決定ボタンを場合
	if (input.IsTriggered(InputId::kA))
	{
		// アイテムがない場合は飛ばす
		if (m_possessItem[m_select] == -1) return shared_from_this();

		Sound::GetInstance().PlayBackSe(SoundName::kSe_useItem);
		UseItem(); // アイテムを使用する
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneUseItem::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kBg], true);
	DrawCursor(); // カーソル表示

	DrawMoney(m_pPlayer); // 所持金額表示

	// アイテムの情報を表示する
	if (m_time < kDispTime) return;
	for (int i = 0; i < m_possessItem.size(); i++)
	{
		// アイテムの情報がない部分は無視する
		if (m_possessItem[i] == -1) continue;
		
		// アイテム画像の表示位置を決める
		Vec2 disp;
		if (i < kSelectCol)
		{
			disp.x = kDispPos.at("itemHandle").x + kItemHandleInterval.x * i;
			disp.y = kDispPos.at("itemHandle").y;
		}
		else
		{
			disp.x = kDispPos.at("itemHandle").x + kItemHandleInterval.x * (i % kSelectCol);
			disp.y = kDispPos.at("itemHandle").y + kItemHandleInterval.y;
		}

		// アイテムの画像を表示する
		DrawRectRotaGraphF(disp.x, disp.y,
			static_cast<int>(kItemSize.x) * m_possessItem[i], 0, static_cast<int>(kItemSize.x), static_cast<int>(kItemSize.y),
			1.0f, 0.0f, m_handle[Handle::kItem], true);

		// 選択中のアイテム名を表示する
		if (m_select == i)
		{
			std::string itemName = m_pItem->GetItemData(m_possessItem[i]).itemName;
			std::string itemExplain = m_pItem->GetItemData(m_possessItem[i]).itemExplain;

			DrawFormatStringFToHandle(kDispPos.at("itemName").x, kDispPos.at("itemName").y, Color::kColorW, 
				Font::m_fontHandle[static_cast<int>(Font::FontId::kMenu_itemName)], "%s", itemName.c_str());

			DrawFormatStringFToHandle(kDispPos.at("itemExplain").x, kDispPos.at("itemExplain").y, Color::kColorW, 
				Font::m_fontHandle[static_cast<int>(Font::FontId::kMenu_itemExplain)], "%s", itemExplain.c_str());
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

	Sound::GetInstance().PlayBackSe(SoundName::kSe_useItem);

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
		dispY = kDispPos.at("cursor").y;
	}
	else
	{
		dispY = kDispPos.at("cursor").y + kCursorMove.y;
	}

	DrawGraphF(kDispPos.at("cursor").x + ((m_select % kSelectCol) * kCursorMove.x), dispY, m_handle[Handle::kCursor], true);
}
