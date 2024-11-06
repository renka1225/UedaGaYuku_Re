#include "DxLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UiBase.h"

// 定数
namespace
{
	const char* kMiniMapPath = "data/ui/minimap.png"; // ミニマップ画像位置のパス
	const char* kCursorPath = "data/ui/cursor.png";   // カーソル画像のパス

	constexpr float kCursorSpeed = 60.0f;		// カーソルの横幅の伸びる量
	constexpr int kTextDisplayTime = 2;			// カーソルの表示間隔
	constexpr int kTextDisplayAnimTime = 240;	// カーソルアニメーションの時間
	constexpr int kMaxCursorAlpha = 255;		// カーソルの最大アルファ値
	constexpr int kMinCursorAlpha = 40;			// カーソルの最小アルファ値
}

UiBase::UiBase():
	m_cursorWidth(0.0f),
	m_cursorDisplayTime(0),
	m_cursorAlpha(kMaxCursorAlpha)
{
	LoadCsv::GetInstance().LoadUiData(m_uiData);
	m_minimapHandle = LoadGraph(kMiniMapPath);
	m_cursorHandle = LoadGraph(kCursorPath);
}

UiBase::~UiBase()
{
	DeleteGraph(m_minimapHandle);
	DeleteGraph(m_cursorHandle);
}

void UiBase::Init()
{
	m_cursorWidth = 0.0f;
}

void UiBase::Update()
{
}

void UiBase::Draw()
{
	// ミニマップ表示
	//DrawRectRotaGraph(30.0f, 800.0f, 0.0f, 0.0f, 1015, 1005, 0.3f, 0.0f, m_minimapHandle, true);
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
	DrawExtendGraphF(dispLTPos.x, dispLTPos.y, dispRBPos.x, dispRBPos.y, m_cursorHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}