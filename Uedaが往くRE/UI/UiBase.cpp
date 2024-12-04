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
	// 画像の種類
	enum Handle
	{
		kCursor,		// カーソル
		kMiniMap,		// ミニマップ
		kIconEnemy,		// ミニマップ上に表示する敵アイコン
		kIconPlayer,	// ミニマップ上に表示するプレイヤーアイコン
		kBattleNow,		// バトル中表示
		kBattleEnd,		// バトル終了演出中表示
		kEnemy_yanki,	// ヤンキー
		kEnemy_tinpira,	// チンピラ
		kEnemy_narikin,	// 成金
		kHandleNum		// 画像の種類
	};

	const char* kHandle[Handle::kHandleNum]
	{
		"data/ui/cursor.png",
		"data/ui/map/minimap.png",
		"data/ui/map/icon_enemy.png",
		"data/ui/map/icon_player.png",
		"data/ui/battle/battleNow.png",
		"data/ui/battle/battleEnd.png",
		"data/ui/battle/yanki.png",
		"data/ui/battle/tinpira.png",
		"data/ui/battle/narikin.png",
	};

	constexpr float kCursorSpeed = 60.0f;		// カーソルの横幅の伸びる量
	constexpr int kTextDisplayTime = 2;			// カーソルの表示間隔
	constexpr int kTextDisplayAnimTime = 240;	// カーソルアニメーションの時間
	constexpr int kMaxCursorAlpha = 255;		// カーソルの最大アルファ値
	constexpr int kMinCursorAlpha = 40;			// カーソルの最小アルファ値

	constexpr float kDispBattleStartMinScale = 1.0f;	 // バトル開始時の敵種類の最小サイズ
	constexpr float kDispBattleStartMaxScale = 10.0f;	 // バトル開始時の敵種類の最大サイズ
	constexpr float kDispBattleStartChangeScale = 0.6f;	 // 表示する敵種類のサイズ変化量
	const Vec2 kDispBattleStartPos = { 900.0f, 500.0f }; // 敵種類表示位置

	const Vec2 kMapPos = { 180.0f, 900.0f };		// マップ表示位置
	constexpr float kWorldWidth = 10000.0f;			// ワールド座標の最大幅
	constexpr float kWorldDepth = 10000.0f;			// ワールド座標の最大奥行き
	constexpr int kMapSize = 1000;					// ミニマップのサイズ
	constexpr int kViewMapSize = 280;				// ミニマップ表示範囲
	constexpr float kIconScale = 0.5f;				// キャラアイコン拡大率

	const Vec2 kBattleNowPos = { 1550.0f, 50.0f };	// バトル中表示位置
	constexpr float kNowBattleMoveSpeed = 13.0f;	// バトル中UIの移動速度

	constexpr int kMaxBlend = 255; // 最大ブレンド率

	const Vec2 kLoadingPos = { 1600.0f, 950.0f };	// ロード中表示位置
	constexpr float kLoadingMoveSpeed = 1.0f;		// テキストの移動速度
	constexpr float kLoadingAmplitude = 4.0f;		// テキストの振幅
	constexpr float kLoadingTextInterval = 20.0f;	// テキストの表示間隔
	constexpr float kLoadingAnimTime = 0.05f;		// ローディング中のアニメーション時間
}

UiBase::UiBase():
	m_cursorWidth(0.0f),
	m_cursorDisplayTime(0),
	m_cursorAlpha(kMaxCursorAlpha),
	m_loadingAnimTime(0.0f),
	m_dispEnemyKindScale(kDispBattleStartMaxScale),
	m_dispNowBattlePosX(Game::kScreenWidth)
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

void UiBase::DrawLoading()
{
	m_loadingAnimTime += kLoadingAnimTime;

	// 表示する文字
	std::string loadingText = "Now Loading...";

	// 文字の表示位置
	float charPosX = kLoadingPos.x; // 文字の間隔を設定

	// 1文字ずつ描画する
	for (size_t i = 0; i < loadingText.size(); i++)
	{
		// 現在の文字
		char currentChar = loadingText[i];

		// 現在の文字幅を取得する
		int charWidth = GetDrawStringWidthToHandle(std::string(1, currentChar).c_str(), 1, Font::m_fontHandle[static_cast<int>(Font::FontId::kLoading)]);

		// sin派を利用して文字を表示する高さを求める
		float offset = sinf(m_loadingAnimTime * kLoadingMoveSpeed + i) * kLoadingAmplitude;
		float charPosY = kLoadingPos.y + offset;

		// 文字を描画
		DrawStringFToHandle(charPosX, charPosY, std::string(1, currentChar).c_str(), Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kLoading)]);

		// 次の文字の描画位置を計算
		charPosX += charWidth;
	}
}

void UiBase::DrawBattleStart()
{
	// 画像のサイズをだんだん小さくする
	m_dispEnemyKindScale -= kDispBattleStartChangeScale;
	m_dispEnemyKindScale = std::max(kDispBattleStartChangeScale, m_dispEnemyKindScale);

	int sizeW, sizeH;
	GetGraphSize(m_handle[Handle::kEnemy_tinpira], &sizeW, &sizeH);
	DrawRectRotaGraphF(kDispBattleStartPos.x, kDispBattleStartPos.y, 0, 0, sizeW, sizeH, m_dispEnemyKindScale, 0.0f, m_handle[Handle::kEnemy_tinpira], true);
}

void UiBase::DrawBattleEnd()
{
	// 乗算で表示する
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMaxBlend);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x1e90ff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 加算で表示する
	SetDrawBlendMode(DX_BLENDMODE_MULA, 200);
	DrawGraph(300, 0, m_handle[Handle::kBattleEnd], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UiBase::DrawBattleUi(const Player& pPlayer)
{
	// バトル中の場合
	if (pPlayer.GetIsBattle())
	{
		// バトル開始時UIを右端から表示する
		m_dispNowBattlePosX -= kNowBattleMoveSpeed;
		m_dispNowBattlePosX = std::max(kBattleNowPos.x, m_dispNowBattlePosX);

		DrawGraphF(m_dispNowBattlePosX, kBattleNowPos.y, m_handle[Handle::kBattleNow], true);
	}
	else
	{
		m_dispNowBattlePosX = Game::kScreenWidth;
	}
}

void UiBase::DrawMiniMap(const Player& pPlayer, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
{
	VECTOR playerPos = pPlayer.GetPos();

	// プレイヤーのワールド座標を正規化（0～1の範囲に変換）
	float normPlayerX = playerPos.x / kWorldWidth;
	float normPlayerY = playerPos.z / kWorldDepth;

	// ミニマップ用の座標に変換
	int mapPlayerX = static_cast<int>(normPlayerX * kMapSize);
	int mapPlayerY = static_cast<int>(normPlayerY * kMapSize);

	// プレイヤーの位置に応じてマップの切り出し位置を変更する
	int srcX = mapPlayerX - kViewMapSize / 2;
	int srcY = mapPlayerY - kViewMapSize / 2;

	// マップをはみ出さないようにする
	srcX = std::clamp(srcX, 0, kMapSize - kViewMapSize);
	srcY = std::clamp(srcY, 0, kMapSize - kViewMapSize);

	// ミニマップ表示
	DrawRectRotaGraphF(kMapPos.x, kMapPos.y, srcX, srcY, kViewMapSize, kViewMapSize, 1.0f, 0.0f, m_handle[Handle::kMiniMap], true);

	// バトル中のみ敵アイコンを表示する
	if (pPlayer.GetIsBattle())
	{
		// 敵のアイコンを表示
		for (auto& enemy : pEnemy)
		{
			VECTOR enemyPos = enemy->GetPos();
			// プレイヤーから敵までの位置ベクトルを計算
			VECTOR pToEVec = VSub(enemyPos, playerPos);

			// 敵のワールド座標を正規化（0～1の範囲に変換）
			float normEnemyX = pToEVec.x / kWorldWidth;
			float normEnemyY = pToEVec.z / kWorldDepth;

			// ミニマップ用の座標に変換
			int mapEnemyX = static_cast<int>(normEnemyX * kMapSize + kMapPos.x);
			int mapEnemyY = static_cast<int>(normEnemyY * kMapSize + kMapPos.y);

			// 敵がミニマップ内にいる場合のみ描画する
			bool isDisp = mapEnemyX >= 0 && mapEnemyX <= kViewMapSize + kMapPos.x + srcX &&
				mapEnemyY >= 0 && mapEnemyY <= kViewMapSize + kMapPos.y + srcY;
			if (isDisp)
			{
				DrawRotaGraph(mapEnemyX, mapEnemyY, kIconScale, enemy->GetAngle(), m_handle[Handle::kIconEnemy], true);
			}
		}
	}

	// プレイヤーのアイコンを表示
	DrawRotaGraphF(kMapPos.x, kMapPos.y, kIconScale, pPlayer.GetAngle(), m_handle[Handle::kIconPlayer], true);
}
