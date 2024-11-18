#include "DxLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UiBase.h"

// 定数
namespace
{
	// 画像の種類
	enum Handle
	{
		kMiniMap,		// ミニマップ
		kCursor,		// カーソル
		kEnemy_yanki,	// ヤンキー
		kEnemy_tinpira,	// チンピラ
		kEnemy_narikin,	// 成金
		kHandleNum		// 画像の種類
	};

	const char* kHandle[Handle::kHandleNum]
	{
		"data/ui/minimap.png",
		"data/ui/cursor.png",
		"data/ui/battleStart/yanki.png",
		"data/ui/battleStart/tinpira.png",
		"data/ui/battleStart/narikin.png",
	};

	constexpr float kCursorSpeed = 60.0f;		// カーソルの横幅の伸びる量
	constexpr int kTextDisplayTime = 2;			// カーソルの表示間隔
	constexpr int kTextDisplayAnimTime = 240;	// カーソルアニメーションの時間
	constexpr int kMaxCursorAlpha = 255;		// カーソルの最大アルファ値
	constexpr int kMinCursorAlpha = 40;			// カーソルの最小アルファ値

	constexpr float kDispBattleStartMinScale = 0.5f;	// バトル開始時の敵種類の最小サイズ
	constexpr float kDispBattleStartMaxScale = 10.0f;	// バトル開始時の敵種類の最大サイズ
	constexpr float kDispBattleStartChangeScale = 0.6f;	// 敵名サイズ変化量
}

UiBase::UiBase():
	m_cursorWidth(0.0f),
	m_cursorDisplayTime(0),
	m_cursorAlpha(kMaxCursorAlpha),
	m_dispEnemyKindScale(kDispBattleStartMaxScale)
{
	LoadCsv::GetInstance().LoadUiData(m_uiData);

	m_handle.resize(Handle::kHandleNum);
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

void UiBase::Update()
{
}

void UiBase::Draw()
{
	// ミニマップ表示
	//DrawRectRotaGraph(30.0f, 800.0f, 0.0f, 0.0f, 1015, 1005, 0.3f, 0.0f, m_handle[Handle::kMiniMap], true);
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

void UiBase::DrawBattleStart()
{
	// TODO:敵の種類によって画像を変える
	auto dispPos = LoadCsv::GetInstance().GetUiData("battle_start");
	//DrawGraphF(dispPos.LTposX, dispPos.LTposY, m_handle[Handle::kEnemy_tinpira], true);

	// 敵名のサイズをだんだん小さくする
	m_dispEnemyKindScale -= kDispBattleStartChangeScale;
	m_dispEnemyKindScale = std::max(kDispBattleStartChangeScale, m_dispEnemyKindScale);

	int sizeW, sizeH;
	GetGraphSize(m_handle[Handle::kEnemy_tinpira], &sizeW, &sizeH);
	DrawRectRotaGraphF(dispPos.LTposX, dispPos.LTposY, 0, 0, sizeW, sizeH, m_dispEnemyKindScale, 0.0f, m_handle[Handle::kEnemy_tinpira], true);
}