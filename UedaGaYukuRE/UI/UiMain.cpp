#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Font.h"
#include "LoadCsv.h"
#include "EnemyBase.h"
#include "SceneMain.h"
#include "UiMain.h"
#include <algorithm>

namespace
{
	/*画像の種類*/
	enum Handle
	{
		kLoading_bg_back,		// ローディング背景
		kLoading_bg_front,		// ローディング背景
		kLoading_triangle_L,	// ローディングの三角形左側
		kLoading_triangle_R,	// ローディングの三角形右側
		kTuto_bg,			// チュートリアル背景
		kTuto_check,		// チュートリアルチェック
		kTuto_ok,			// チュートリアル完了
		kTuto_1,			// チュートリアル1
		kTuto_2,			// チュートリアル2
		kTuto_3,			// チュートリアル3
		kTuto_4,			// チュートリアル4
		kTuto_5,			// チュートリアル5
		kMoney,				// 所持金
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

	/*紹介画像*/
	enum IntroduceHandle
	{
		kUeda,
		kSaionzi,
		kBob,
		kSato,
		kAbe,
		kOhara,
		kIntroduceNum
	};

	/*心得*/
	enum Knowledge
	{
		kZero,
		kOne,
		kTwo,
		kThree,
		kFour,
		kKnowledgeNum
	};

	/*画像パス*/
	const char* kHandle[Handle::kNum]
	{
		"data/ui/load/bg_back.png",
		"data/ui/load/bg_front.png",
		"data/ui/load/triangle_L.png",
		"data/ui/load/triangle_R.png",
		"data/ui/tutorial/bg.png",
		"data/ui/tutorial/check.png",
		"data/ui/tutorial/ok.png",
		"data/ui/tutorial/tuto1.png",
		"data/ui/tutorial/tuto2.png",
		"data/ui/tutorial/tuto3.png",
		"data/ui/tutorial/tuto4.png",
		"data/ui/tutorial/tuto5.png",
		"data/ui/bg_money.png",
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

	/*ロード画像パス*/
	const char* kIntroduceHandle[IntroduceHandle::kIntroduceNum]
	{
		"data/ui/load/text_ueda.png",
		"data/ui/load/text_saionzi.png",
		"data/ui/load/text_bob.png",
		"data/ui/load/text_sato.png",
		"data/ui/load/text_abe.png",
		"data/ui/load/text_ohara.png"
	};
	
	/*心得画像パス*/
	const char* kKnowledgeHandle[Knowledge::kKnowledgeNum]
	{
		"data/ui/tutorial/synopsis.png",
		"data/ui/tutorial/knowledge_1.png",
		"data/ui/tutorial/knowledge_2.png",
		"data/ui/tutorial/knowledge_3.png",
		"data/ui/tutorial/knowledge_4.png",
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
		{"heat", {1768.0f, 344.0f}},
		{"bg", { 1433.0f, 238.0f }},	// 背景
		{"text", { 1433.0f, 237.0f }},	// テキスト
		{"check", { 1450.0f, 339.0f }},	// チェックマーク
		{"ok",{1432.0f, 422.0f}},		// OK
		{"knowledge", {352.0f,164.0f}}	// 心得
	};

	constexpr float kTutoCheckHeight = 84.0f;	// チェックマーク表示間隔
	constexpr int kBgAlpha = 200;				// 背景のブレンド率

	/*ロード*/
	// 紹介画像表示位置
	const Vec2 kIntroducePos[IntroduceHandle::kIntroduceNum]
	{
		{ 542.0f, 329.0f },
		{ 476.0f, 329.0f },
		{ 473.0f, 329.0f },
		{ 462.0f, 329.0f },
		{ 459.0f, 329.0f },
		{ 432.0f, 326.0f }
	};

	const Vec2 kLoadingPos = { 1600.0f, 950.0f };				// "NowLoading..."表示位置
	const Vec2 kLoadingTrianglePos = { 370.0f, 517.0f };		// 三角形表示位置
	const Vec2 kLoadingDispIntroduceSize = { 571.0f, 357.0f };	// キャラクター紹介文表示サイズ
	constexpr int kLoadingIntroduceNum = 6;						// キャラクター紹介文数
	constexpr int kLoadingIntroduceChangeTime = 180;			// キャラクター紹介の変更時間
	constexpr float kLoadingTriangleMove = 10.0f;				// 三角形の動く幅
	constexpr float kLoadingMoveSpeed = 1.0f;					// テキストの移動速度
	constexpr float kLoadingAmplitude = 4.0f;					// テキストの振幅
	constexpr float kLoadingTextInterval = 20.0f;				// テキストの表示間隔
	constexpr float kLoadingAnimTime = 0.05f;					// ローディング中のアニメーション時間

	/*操作説明*/
	const Vec2 kDispOperationPos = { 1635.0f, 905.0f };			// 通常操作説明表示位置
	const Vec2 kDispBattleOperationPos = { 1584.0f, 700.0f };	// バトル中操作説明表示位置

	/*会話*/
	const Vec2 kDispTalkUiPos = { -5.0f, 32.0f }; // "話す"テキスト表示位置調整

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
	constexpr float kDispBattleStartChangeScale = 0.8f;	 // 表示する敵種類のサイズ変化量
	const Vec2 kDispBattleStartPos = { 950.0f, 700.0f }; // 敵種類表示位置

	const Vec2 kBattleEndBgPos = { 200, 0 };		// バトル終了時の背景位置
	const Vec2 kGekihaTextPos = { 950, 500 };		// "撃破"テキスト位置
	constexpr int kGekihaDispTime = 80;				//  撃破UIを表示しはじめる時間
	constexpr float kGekihaTextMinScale = 1.0f;		// "撃破"テキスト最小サイズ
	constexpr float kGekihaTextMaxScale = 10.0f;	// "撃破"テキスト最大サイズ
	constexpr float kGekihaTextChangeScale = 0.6f;	// "撃破"テキストサイズ

	const Vec2 kBattleNowPos = { 1550.0f, 50.0f };	// バトル中表示位置
	constexpr float kNowBattleMoveSpeed = 13.0f;	// バトル中UIの移動速度

	/*所持金*/
	const Vec2 kDispMoneyInitPos = { 1329.0f, -120.0f }; // 所持金UI初期位置
	const Vec2 kDispMoneyText = { 470.0f, 30.0f };		 // 所持金テキスト表示位置
	constexpr float kDispMoneyPosY = 18.0f;				 // UI移動後のY座標表示位置
	constexpr int kMoneyAnimTotalTime = 150;			 // 所持金UIの合計アニメーション時間
	constexpr int kDispMoneyTime = 110;					 // 所持金UIを表示する時間
	constexpr int kDispMoneyMoveSpeed = 15;				 // 所持金UIを移動させるスピード

	constexpr int kMaxBlend = 255; // 最大ブレンド率
}

UiMain::UiMain() :
	m_dispIntroducePos({ kLoadingDispIntroduceSize.x, kLoadingDispIntroduceSize.y }),
	m_dispMoneyPos(kDispMoneyInitPos),
	m_loadingTime(0.0f),
	m_loadingAnimTime(0.0f),
	m_dispMoneyAnimTime(0),
	m_dispGekihaTextScale(kDispBattleTextMaxScale),
	m_dispEnemyKindScale(kDispBattleTextMaxScale),
	m_dispNowBattlePosX(Game::kScreenWidth),
	m_nowLoadingIntroduce(GetRand(IntroduceHandle::kIntroduceNum - 1))
{
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandle[i]);
	}

	m_introduceHandle.resize(IntroduceHandle::kIntroduceNum);
	for (int i = 0; i < m_introduceHandle.size(); i++)
	{
		m_introduceHandle[i] = LoadGraph(kIntroduceHandle[i]);
	}

	m_knowledgeHandle.resize(Knowledge::kKnowledgeNum);
	for (int i = 0; i < m_knowledgeHandle.size(); i++)
	{
		m_knowledgeHandle[i] = LoadGraph(kKnowledgeHandle[i]);
	}
}


UiMain::~UiMain()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
	for (auto& handle : m_introduceHandle)
	{
		DeleteGraph(handle);
	}
	for (auto& handle : m_knowledgeHandle)
	{
		DeleteGraph(handle);
	}
}

void UiMain::UpdateLoading(const Input& input)
{
	m_loadingTime++;

	// カーソルで画像を切り替える
	if (input.IsTriggered(InputId::kLeft))
	{
		// 画像を左側に移動させる
		if (m_nowLoadingIntroduce == 0)
		{
			m_nowLoadingIntroduce = kLoadingIntroduceNum - 1; // 最後の画像に移動
		}
		else
		{
			m_nowLoadingIntroduce--;
		}

		m_loadingTime = 0;
	}
	else if (input.IsTriggered(InputId::kRight))
	{
		// 画像を右側に移動させる
		if (m_nowLoadingIntroduce == kLoadingIntroduceNum - 1)
		{
			m_nowLoadingIntroduce = 0; // 最初の画像に移動
		}
		else
		{
			m_nowLoadingIntroduce++;
		}

		m_loadingTime = 0;
	}

	// 数秒ごとに紹介画像を切り替える
	if (m_loadingTime >= kLoadingIntroduceChangeTime)
	{
		// 次の画像に変える
		m_nowLoadingIntroduce = (m_nowLoadingIntroduce + 1) % kLoadingIntroduceNum;
		m_loadingTime = 0;
	}

	// テキストを左に移動させる
	//m_dispIntroducePos.x = kLoadingTextPos.x;

}

void UiMain::DrawLoading()
{
	/*背景表示*/
	// 最背面
	DrawGraph(0, 0, m_handle[Handle::kLoading_bg_back], true);

	DrawGraphF(kIntroducePos[m_nowLoadingIntroduce].x, kIntroducePos[m_nowLoadingIntroduce].y, m_introduceHandle[m_nowLoadingIntroduce], true);

	// 最前面
	DrawGraph(0, 0, m_handle[Handle::kLoading_bg_front], true);

	// 三角形
	float movePos = sinf(m_loadingAnimTime) * kLoadingTriangleMove; // 左右に動かす
	DrawGraphF(kLoadingTrianglePos.x - movePos, kLoadingTrianglePos.y, m_handle[Handle::kLoading_triangle_L], true);
	DrawGraphF((Game::kScreenWidth - kLoadingTrianglePos.x) + movePos, kLoadingTrianglePos.y, m_handle[Handle::kLoading_triangle_R], true);

	/*Loadingの文字表示*/
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

#ifdef _DEBUG
	DrawFormatString(0, 20, Color::kColorW, "表示画像:%d", m_nowLoadingIntroduce);
#endif
}

void UiMain::DrawBattleStart()
{
	// 画像のサイズをだんだん小さくする
	m_dispEnemyKindScale -= kDispBattleStartChangeScale;
	m_dispEnemyKindScale = std::max(kDispBattleTextMinScale, m_dispEnemyKindScale);

	// TODO:敵の種類によって表示を変える
	int sizeW, sizeH;
	GetGraphSize(m_handle[Handle::kEnemy_tinpira], &sizeW, &sizeH);
	DrawRectRotaGraphF(kDispBattleStartPos.x, kDispBattleStartPos.y, 0, 0, sizeW, sizeH, m_dispEnemyKindScale, 0.0f, m_handle[Handle::kEnemy_tinpira], true);
}

void UiMain::DrawBattleEnd(int time)
{
	if (time > kGekihaDispTime) return; // 一定時間経ったら表示する
	// 乗算で表示する
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMaxBlend);
	DrawGraphF(kBattleEndBgPos.x, kBattleEndBgPos.y, m_handle[Handle::kBattle_end], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

void UiMain::SetAnimMoneyUi()
{
	// アニメーション再生中の場合は初期化しない
	if (m_dispMoneyAnimTime > 0) return;

	m_dispMoneyAnimTime = kMoneyAnimTotalTime;
	m_dispMoneyPos = kDispMoneyInitPos;
}

void UiMain::DrawMoneyUi(int money)
{
	// アニメーション再生中でない場合は飛ばす
	if (m_dispMoneyAnimTime <= 0) return;

	m_dispMoneyAnimTime--;

	// 所持金UIを下に移動させる
	if (m_dispMoneyAnimTime >= kMoneyAnimTotalTime - kDispMoneyTime)
	{
		m_dispMoneyPos.y += kDispMoneyMoveSpeed;
		m_dispMoneyPos.y = std::min(kDispMoneyPosY, m_dispMoneyPos.y);
	}
	else
	{
		m_dispMoneyPos.y -= kDispMoneyMoveSpeed;
		m_dispMoneyPos.y = std::max(m_dispMoneyPos.y, kDispMoneyInitPos.y);
	}

	DrawGraphF(m_dispMoneyPos.x, m_dispMoneyPos.y, m_handle[Handle::kMoney], true);

	// 現在の所持金額を取得
	int man = money / 10000; // 万単位
	int yen = money % 10000; // 円単位

	// 表示用のテキスト
	std::string moneyText;

	// 万の表示
	if (man > 0)
	{
		moneyText += std::to_string(man) + "万";
		moneyText += std::to_string(yen) + "円";
	}
	// 円の表示のみ
	else
	{
		moneyText = std::to_string(yen) + "円";
	}

	// 金額部分を右詰めにする
	int moneyWidth = GetDrawStringWidthToHandle(moneyText.c_str(), static_cast<int>(moneyText.size()), Font::m_fontHandle[static_cast<int>(Font::FontId::kMoney)]);
	float dispX = m_dispMoneyPos.x - moneyWidth + kDispMoneyText.x;

	// 現在の所持金額を表示
	DrawStringFToHandle(dispX, m_dispMoneyPos.y + kDispMoneyText.y, moneyText.c_str(), Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kMoney)]);
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
	// バトル中
	if (isBattle)
	{
		DrawGraphF(kDispBattleOperationPos.x, kDispBattleOperationPos.y, m_handle[Handle::kOperation_battle], true);
	}
	else
	{
		DrawGraphF(kDispOperationPos.x, kDispOperationPos.y, m_handle[Handle::kOperation_normal], true);
	}
}

void UiMain::DrawTutorial(Player::TutorialInfo tutoInfo)
{
	// チュートリアル背景
	if (tutoInfo.currentNum >= Player::TutorialNum::kTuto_1)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, kBgAlpha);
		DrawGraphF(kDispTutoPos.at("bg").x, kDispTutoPos.at("bg").y, m_handle[Handle::kTuto_bg], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	switch (tutoInfo.currentNum)
	{
	case Player::TutorialNum::kTuto_0:
		DrawTutoKnowledge(tutoInfo);
		break;
	case Player::TutorialNum::kTuto_1:
		DrawTuto1(tutoInfo);
		break;
	case Player::TutorialNum::kTuto_2:
		DrawTuto2(tutoInfo);
		break;
	case Player::TutorialNum::kTuto_3:
		DrawTuto3(tutoInfo);
		break;
	case Player::TutorialNum::kTuto_4:
		DrawTuto4(tutoInfo);
		break;
	case Player::TutorialNum::kTuto_5:
		DrawTuto5(tutoInfo);
		break;
	}

	// チュートリアル完了の表示
	if (tutoInfo.tutorialChangeTime > 0)
	{
		DrawTutorialOk();
	}
}

void UiMain::DrawTuto1(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kDispTutoPos.at("text").x, kDispTutoPos.at("text").y, m_handle[Handle::kTuto_1], true);

	if (tutoInfo.isMove)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y);
	}
	if (tutoInfo.isDush)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight);
	}
	if (tutoInfo.isCameraMove)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight * 2);
	}

	DrawTutoKnowledge(tutoInfo);
}

void UiMain::DrawTuto2(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kDispTutoPos.at("text").x, kDispTutoPos.at("text").y, m_handle[Handle::kTuto_2], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("punch"), tutoInfo.currentPunch);
	DrawTutorialCurrentNum(kDispTutoPos.at("kick"), tutoInfo.currentKick);
	DrawTutorialCurrentNum(kDispTutoPos.at("avoid"), tutoInfo.currentAvoid);
	DrawTutorialCurrentNum(kDispTutoPos.at("guard"), tutoInfo.currentGuard);

	if (tutoInfo.isPunch)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y);
	}
	if (tutoInfo.isKick)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight);
	}
	if (tutoInfo.isAvoid)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight * 2);
	}
	if (tutoInfo.isGuard)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight * 3);
	}
}

void UiMain::DrawTuto3(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kDispTutoPos.at("text").x, kDispTutoPos.at("text").y, m_handle[Handle::kTuto_3], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("grab"), tutoInfo.currentGrab);
	DrawTutorialCurrentNum(kDispTutoPos.at("weaponAtk"), tutoInfo.currentWeaponAtk);

	if (tutoInfo.isGrab)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y);
	}
	if (tutoInfo.isWeaponAtk)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y + kTutoCheckHeight);
	}

	DrawTutoKnowledge(tutoInfo);
}

void UiMain::DrawTuto4(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kDispTutoPos.at("text").x, kDispTutoPos.at("text").y, m_handle[Handle::kTuto_4], true);

	DrawTutorialCurrentNum(kDispTutoPos.at("heat"), tutoInfo.currentHeat);

	if (tutoInfo.isHeat)
	{
		DrawTutorialCheck(kDispTutoPos.at("check").y);
	}
}

void UiMain::DrawTuto5(Player::TutorialInfo tutoInfo)
{
	DrawGraphF(kDispTutoPos.at("text").x, kDispTutoPos.at("text").y, m_handle[Handle::kTuto_5], true);
}

void UiMain::DrawTutorialCheck(float posY)
{
	DrawGraphF(kDispTutoPos.at("check").x, posY, m_handle[Handle::kTuto_check], true);
}

void UiMain::DrawTutorialOk()
{
	DrawGraphF(kDispTutoPos.at("ok").x, kDispTutoPos.at("ok").y, m_handle[Handle::kTuto_ok], true);
}

void UiMain::DrawTutorialCurrentNum(Vec2 pos, int currentNum)
{
	DrawFormatStringFToHandle(pos.x, pos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutorial)], "%d", currentNum);
}

void UiMain::DrawTutoKnowledge(Player::TutorialInfo tutoInfo)
{
	if (!tutoInfo.isNowKnowledge) return;

	DrawGraphF(kDispTutoPos.at("knowledge").x, kDispTutoPos.at("knowledge").y, m_knowledgeHandle[tutoInfo.currentKnowledge], true);
}