#include "DxLib.h"
#include "Game.h"
#include "Font.h"
#include "LoadCsv.h"
#include "Player.h"
#include "EnemyBase.h"
#include "UiBase.h"
#include <algorithm>

// 定数
namespace
{
	/*画像の種類*/
	enum Handle
	{
		kCursor,		// カーソル
		kSaveCursor,	// セーブ用のカーソル
		kNum			// 画像の種類
	};

	const char* kHandle[Handle::kNum]
	{
		"data/ui/cursor.png",
		"data/ui/save/cursor.png",
	};

	/*カーソル*/
	constexpr float kCursorSpeed = 60.0f;		// カーソルの横幅の伸びる量
	constexpr int kTextDisplayTime = 2;			// カーソルの表示間隔
	constexpr int kTextDisplayAnimTime = 240;	// カーソルアニメーションの時間
	constexpr int kMaxCursorAlpha = 255;		// カーソルの最大アルファ値
	constexpr int kMinCursorAlpha = 40;			// カーソルの最小アルファ値
	constexpr float kSaveCursorMove = 212.0f;	// セーブ画面のカーソル移動量
}

UiBase::UiBase() :
	m_cursorWidth(0.0f),
	m_cursorDisplayTime(0),
	m_cursorAlpha(kMaxCursorAlpha)
{
	LoadCsv::GetInstance().LoadUiData(m_uiData);

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandle[i]);
	}
}

UiBase::~UiBase()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void UiBase::Init()
{
	m_cursorWidth = 0.0f;
}

void UiBase::UpdateCursor(std::string cursorId)
{
	// カーソルの横幅を更新
	m_cursorWidth += kCursorSpeed;

	// シーンによってカーソルの長さを変える
	m_cursorWidth = std::min(m_cursorWidth, m_uiData[cursorId].width);
	
	// カーソルのα値を更新
	m_cursorDisplayTime += kTextDisplayTime;
	m_cursorDisplayTime %= kTextDisplayAnimTime;

	// MEMO:sin波を使って0～1の範囲にする
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_cursorDisplayTime) / kTextDisplayAnimTime * DX_PI_F);
	m_cursorAlpha = kMinCursorAlpha + static_cast<int>((kMaxCursorAlpha - kMinCursorAlpha) * sinAlpha);
}

void UiBase::DrawCursor(std::string cursorId, int select, float interval)
{
	Vec2 dispLTPos = { m_uiData[cursorId].LTposX, m_uiData[cursorId].LTposY + interval * select };
	Vec2 dispRBPos = { m_uiData[cursorId].LTposX + m_cursorWidth, m_uiData[cursorId].LTposY + interval * select + m_uiData[cursorId].height };
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_cursorAlpha);
	DrawExtendGraphF(dispLTPos.x, dispLTPos.y, dispRBPos.x, dispRBPos.y, m_handle[Handle::kCursor], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UiBase::DrawSaveCursor(Vec2 pos, int select)
{
	pos.y = pos.y + (select * kSaveCursorMove);
	DrawGraphF(pos.x, pos.y, m_handle[Handle::kSaveCursor], true);
}
