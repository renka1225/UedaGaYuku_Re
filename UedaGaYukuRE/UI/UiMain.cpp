#include "DxLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Font.h"
#include "EnemyBase.h"
#include "UiMain.h"
#include <algorithm>

namespace
{
	/*画像の種類*/
	enum Handle
	{
		kTuto_bg,			// チュートリアル背景
		kTuto_check,		// チュートリアルチェック
		kTuto_1,			// チュートリアル1
		kTuto_2,			// チュートリアル2
		kTuto_3,			// チュートリアル3
		kTuto_4,			// チュートリアル4
		kTextBox,			// テキストボックス
		kMiniMap,			// ミニマップ
		kIconEnemy,			// ミニマップ上に表示する敵アイコン
		kIconPlayer,		// ミニマップ上に表示するプレイヤーアイコン
		kNpcTalk,			// 話すのUI
		kOperation_normal,	// 操作説明
		kOperation_battle,	// バトル時操作説明
		kBattle_now,		// バトル中
		kBattle_end,		// バトル終了
		kBattle_lose,		// バトルで負けた
		kBattle_gekiha,		// 撃破
		kEnemy_yanki,		// ヤンキー
		kEnemy_tinpira,		// チンピラ
		kEnemy_narikin,		// 成金
		kNum				// 画像の種類
	};

	const char* kHandle[Handle::kNum]
	{
		"data/ui/tutorial/bg.png",
		"data/ui/tutorial/check.png",
		"data/ui/tutorial/tuto1.png",
		"data/ui/tutorial/tuto2.png",
		"data/ui/tutorial/tuto3.png",
		"data/ui/tutorial/tuto4.png",
		"data/ui/main/textBox.png",
		"data/ui/main/minimap.png",
		"data/ui/main/icon_enemy.png",
		"data/ui/main/icon_player.png",
		"data/ui/text/hanasu.png",
		"data/ui/main/operation_normal.png",
		"data/ui/main/operation_battle.png",
		"data/ui/battle/battleNow.png",
		"data/ui/battle/battleEnd.png",
		"data/ui/battle/battleLose.png",
		"data/ui/battle/gekiha.png",
		"data/ui/battle/yanki.png",
		"data/ui/battle/tinpira.png",
		"data/ui/battle/narikin.png",
	};

	/*チュートリアル*/
	// 表示位置
	const std::map<std::string, Vec2> kDispTutoPos
	{
		{"punch", {1745.0f, 346.0f}},
		{"kick", {1749.0f, 433.0f}},
		{"avoid", {1670.0f, 519.0f}},
		{"guard", {1698.0f, 604.0f}},
		{"grab", {1814.0f, 345.0f}},
		{"weaponAtk", {1824.0f, 432.0f}},
		{"heat", {1828.0f, 344.0f}},
	};

	constexpr int kBgAlpha = 200;	// 背景のブレンド率
	const Vec2 kTutoBgPos = { 1433.0f, 238.0f };	// 背景位置
	const Vec2 kTutoTextPos = { 1433.0f, 237.0f };	// テキスト位置
	const Vec2 kTutoCheckPos = { 1450.0f, 339.0f };	// チェックマーク位置
	constexpr float kTutoCheckHeight = 84.0f;		// チェックマーク表示間隔

	/*操作説明*/
	const Vec2 kDispOperationPos = { 1635.0f, 905.0f };			// 通常操作説明表示位置
	const Vec2 kDispBattleOperationPos = { 1584.0f, 700.0f };	// バトル中操作説明表示位置

	/*会話*/
	const Vec2 kDispTalkUiPos = { -5.0f, 32.0f }; // "話す"テキスト表示位置調整
	const Vec2 kTextBoxPos = { 116.0f, 766.0f };  // テキストボックス位置
	const Vec2 kTalkNamePos = { 287.0f, 775.0f }; // 名前表示位置
	const Vec2 kTalkPos = { 489.0f, 850.0f };	  // テキスト表示位置

	/*ミニマップ*/
	const Vec2 kMapPos = { 180.0f, 900.0f };	// マップ表示位置
	constexpr float kWorldWidth = 13740.0f;		// ワールド座標の最大幅
	constexpr float kWorldDepth = 11220.0f;		// ワールド座標の最大奥行き
	constexpr int kMapSize = 1000;				// ミニマップのサイズ
	constexpr int kViewMapSize = 280;			// ミニマップ表示範囲
	constexpr float kViewEnemyIcon = 500.0f;	// 敵アイコンの表示範囲
	constexpr float kIconScale = 0.5f;			// キャラアイコン拡大率

	/*バトル*/
	constexpr float kDispBattleTextMinScale = 1.0f;		 // バトル演出テキストの最小サイズ
	constexpr float kDispBattleTextMaxScale = 15.0f;	 // バトル演出テキストの最大サイズ
	constexpr float kDispBattleStartChangeScale = 0.6f;	 // 表示する敵種類のサイズ変化量
	const Vec2 kDispBattleStartPos = { 950.0f, 700.0f }; // 敵種類表示位置

	const Vec2 kBattleEndBgPos = { 200, 0 };		// バトル終了時の背景位置
	const Vec2 kGekihaTextPos = { 950, 500 };		// "撃破"テキスト位置
	constexpr int kGekihaDispTime = 80;				// "撃破"テキストを表示しはじめる時間
	constexpr float kGekihaTextMinScale = 1.0f;		// "撃破"テキスト最小サイズ
	constexpr float kGekihaTextMaxScale = 10.0f;	// "撃破"テキスト最大サイズ
	constexpr float kGekihaTextChangeScale = 0.6f;	// "撃破"テキストサイズ

	const Vec2 kBattleNowPos = { 1550.0f, 50.0f };	// バトル中表示位置
	constexpr float kNowBattleMoveSpeed = 13.0f;	// バトル中UIの移動速度

	constexpr int kMaxBlend = 255; // 最大ブレンド率

	/*ロード*/
	const Vec2 kLoadingPos = { 1600.0f, 950.0f };	// ロード中表示位置
	constexpr float kLoadingMoveSpeed = 1.0f;		// テキストの移動速度
	constexpr float kLoadingAmplitude = 4.0f;		// テキストの振幅
	constexpr float kLoadingTextInterval = 20.0f;	// テキストの表示間隔
	constexpr float kLoadingAnimTime = 0.05f;		// ローディング中のアニメーション時間
}

UiMain::UiMain():
	m_loadingAnimTime(0.0f),
	m_dispGekihaTextScale(kDispBattleTextMaxScale),
	m_dispEnemyKindScale(kDispBattleTextMaxScale),
	m_dispNowBattlePosX(Game::kScreenWidth)
{
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandle[i]);
	}
}

UiMain::~UiMain()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void UiMain::DrawLoading()
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

		DrawStringFToHandle(charPosX, charPosY, std::string(1, currentChar).c_str(), Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kLoading)]);

		// 次の文字の描画位置を計算
		charPosX += charWidth;
	}
}

void UiMain::DrawBattleStart()
{
	// 画像のサイズをだんだん小さくする
	m_dispEnemyKindScale -= kDispBattleStartChangeScale;
	m_dispEnemyKindScale = std::max(kDispBattleTextMinScale, m_dispEnemyKindScale);

	int sizeW, sizeH;
	GetGraphSize(m_handle[Handle::kEnemy_tinpira], &sizeW, &sizeH);
	DrawRectRotaGraphF(kDispBattleStartPos.x, kDispBattleStartPos.y, 0, 0, sizeW, sizeH, m_dispEnemyKindScale, 0.0f, m_handle[Handle::kEnemy_tinpira], true);
}

void UiMain::DrawBattleEnd(int time)
{
	// 乗算で表示する
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMaxBlend);
	DrawGraphF(kBattleEndBgPos.x, kBattleEndBgPos.y, m_handle[Handle::kBattle_end], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//if (time < kGekihaDispTime)
	//{
	//	// テキストのサイズをだんだん小さくする
	//	m_dispGekihaTextScale -= kGekihaTextChangeScale;
	//	m_dispGekihaTextScale = std::max(kDispBattleTextMinScale, m_dispGekihaTextScale);

	//	int sizeW, sizeH;
	//	GetGraphSize(m_handle[Handle::kBattle_gekiha], &sizeW, &sizeH);
	//	DrawRectRotaGraphF(kGekihaTextPos.x, kGekihaTextPos.y, 0, 0, sizeW, sizeH, m_dispGekihaTextScale, 0.0f, m_handle[Handle::kBattle_gekiha], true);
	//}
}

void UiMain::DrawBattleLose()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMaxBlend);
	DrawGraph(0, 0, m_handle[Handle::kBattle_lose], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UiMain::DrawEnding()
{
	// 背景表示
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, Color::kColorBK, true);

#ifdef _DEBUG
	DrawString(0, 0, "エンディング中\n", Color::kColorW);
#endif
}

void UiMain::DrawNpcUi(VECTOR pos)
{
	// NPCの位置からUIの位置を決める
	VECTOR modelTopPos = VAdd(pos, VGet(kDispTalkUiPos.x, kDispTalkUiPos.y, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(modelTopPos);
	bool isViewClip = CheckCameraViewClip(modelTopPos); // カメラの視界内に入っているか(true:視界に入っていない)

	if (!isViewClip)
	{
		DrawGraphF(screenPos.x, screenPos.y, m_handle[Handle::kNpcTalk], true);
	}
}

void UiMain::DrawBattleUi(const Player& pPlayer)
{
	// バトル中の場合
	if (pPlayer.GetIsBattle())
	{
		// バトル開始時UIを右端から表示する
		m_dispNowBattlePosX -= kNowBattleMoveSpeed;
		m_dispNowBattlePosX = std::max(kBattleNowPos.x, m_dispNowBattlePosX);

		DrawGraphF(m_dispNowBattlePosX, kBattleNowPos.y, m_handle[Handle::kBattle_now], true);
	}
	else
	{
		m_dispNowBattlePosX = Game::kScreenWidth;
	}
}

void UiMain::DrawMiniMap(const Player& pPlayer, std::vector<std::shared_ptr<EnemyBase>> pEnemy)
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

	// 敵のアイコンを表示
	for (auto& enemy : pEnemy)
	{
		if (enemy == nullptr) continue;

		VECTOR enemyPos = enemy->GetPos();
		// プレイヤーから敵までの位置ベクトルを計算
		VECTOR pToEVec = VSub(enemyPos, playerPos);

		// 敵のワールド座標を正規化（0～1の範囲に変換）
		float normEnemyX = pToEVec.x / kWorldWidth;
		float normEnemyY = pToEVec.z / kWorldDepth;

		// ミニマップ用の座標に変換
		int mapEnemyX = static_cast<int>(normEnemyX * kMapSize + kMapPos.x);
		int mapEnemyY = static_cast<int>(normEnemyY * kMapSize + kMapPos.y);

		// プレイヤーの近くにいる場合のみ描画する
		bool isDisp = VSize(pToEVec) < kViewEnemyIcon;
		if (isDisp)
		{
			DrawRotaGraph(mapEnemyX, mapEnemyY, kIconScale, enemy->GetAngle(), m_handle[Handle::kIconEnemy], true);
		}
	}

	// プレイヤーのアイコンを表示
	DrawRotaGraphF(kMapPos.x, kMapPos.y, kIconScale, pPlayer.GetAngle(), m_handle[Handle::kIconPlayer], true);
}

void UiMain::DrawOperation(bool isBattle)
{
	if (isBattle)
	{
		DrawGraphF(kDispBattleOperationPos.x, kDispBattleOperationPos.y, m_handle[Handle::kOperation_battle], true);
	}
	else
	{
		DrawGraphF(kDispOperationPos.x, kDispOperationPos.y, m_handle[Handle::kOperation_normal], true);
	}
}

void UiMain::DrawTalk(const Player& pPlayer, std::string id, int clearNum)
{
	// テキストボックス表示
	DrawGraphF(kTextBoxPos.x, kTextBoxPos.y, m_handle[Handle::kTextBox], true);

	// 名前表示
	std::string drawName = LoadCsv::GetInstance().GetConversationName(id);
	DrawFormatStringFToHandle(kTalkNamePos.x, kTalkNamePos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTalk_Name)], drawName.c_str());

	// テキスト表示
	std::string drawText = LoadCsv::GetInstance().GetConversationText(id);
	
	int drawNum = 0;
	if (id == "BOSS_OK")
	{
		drawNum = pPlayer.GetDeadEnemyNum();
	}
	else if(id == "BOSS_NG")
	{
		drawNum = clearNum - pPlayer.GetDeadEnemyNum();
	}

	DrawFormatStringFToHandle(kTalkPos.x, kTalkPos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTalk)], drawText.c_str(), drawNum);
}

void UiMain::DrawTutorial(Player::TutorialInfo tutoInfo)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kBgAlpha);
	DrawGraphF(kTutoBgPos.x, kTutoBgPos.y, m_handle[Handle::kTuto_bg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	switch (tutoInfo.currentNum)
	{
	// チュートリアル1
	case Player::TutorialNum::kTuto_1:
		DrawTuto1(tutoInfo);
		break;
	// チュートリアル2
	case Player::TutorialNum::kTuto_2:
		DrawTuto2(tutoInfo);
		break;
	// チュートリアル3
	case Player::TutorialNum::kTuto_3:
		DrawTuto3(tutoInfo);
		break;
	// チュートリアル4
	case Player::TutorialNum::kTuto_4:
		DrawTuto4(tutoInfo);
		break;
	}
}

void UiMain::DrawTuto1(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kTutoTextPos.x, kTutoTextPos.y, m_handle[Handle::kTuto_1], true);

	if (tutoInfo.isMove)
	{
		DrawTutorialCheck(kTutoCheckPos.y);
	}
	if (tutoInfo.isDush)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight);
	}
	if (tutoInfo.isCameraMove)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight * 2);
	}
}

void UiMain::DrawTuto2(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kTutoTextPos.x, kTutoTextPos.y, m_handle[Handle::kTuto_2], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("punch"), tutoInfo.currentPunch);
	DrawTutorialCurrentNum(kDispTutoPos.at("kick"), tutoInfo.currentKick);
	DrawTutorialCurrentNum(kDispTutoPos.at("avoid"), tutoInfo.currentAvoid);
	DrawTutorialCurrentNum(kDispTutoPos.at("guard"), tutoInfo.currentGuard);

	if (tutoInfo.isPunch)
	{
		DrawTutorialCheck(kTutoCheckPos.y);
	}
	if (tutoInfo.isKick)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight);
	}
	if (tutoInfo.isAvoid)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight * 2);
	}
	if (tutoInfo.isGuard)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight * 3);
	}
}

void UiMain::DrawTuto3(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kTutoTextPos.x, kTutoTextPos.y, m_handle[Handle::kTuto_3], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("grab"), tutoInfo.currentGrab);
	DrawTutorialCurrentNum(kDispTutoPos.at("weaponAtk"), tutoInfo.currentWeaponAtk);

	if (tutoInfo.isGrab)
	{
		DrawTutorialCheck(kTutoCheckPos.y);
	}
	if (tutoInfo.isWeaponAtk)
	{
		DrawTutorialCheck(kTutoCheckPos.y + kTutoCheckHeight);
	}
}

void UiMain::DrawTuto4(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kTutoTextPos.x, kTutoTextPos.y, m_handle[Handle::kTuto_4], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("heat"), tutoInfo.currentHeat);

	if (tutoInfo.isHeat)
	{
		DrawTutorialCheck(kTutoCheckPos.y);
	}
}

void UiMain::DrawTutorialCheck(float posY)
{
	DrawGraphF(kTutoCheckPos.x, posY, m_handle[Handle::kTuto_check], true);
}

void UiMain::DrawTutorialCurrentNum(Vec2 pos, int currentNum)
{
	DrawFormatStringFToHandle(pos.x, pos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutorial)], "%d", currentNum);
}
