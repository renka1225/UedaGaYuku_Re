#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Font.h"
#include "Sound.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "Camera.h"
#include "PlayerSelect.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

namespace
{
	// 画像の種類
	enum Handle
	{
		kSelectBg,		// 背景
		kSelectBgUnder,	// 背景の下部分
		kSaveBg,		// セーブ画面の背景
		kSelectText,	// テキスト
		kCopyright,		// 権利表記
		kEndSelect,		// ゲームを終了するかの選択肢
		kNum			// ハンドルの数
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/select/bg.png",
		"data/ui/select/bg_under.png",
		"data/ui/select/save.png",
		"data/ui/select/text.png",
		"data/ui/select/copyright.png",
		"data/ui/select/endSelect.png"
	};

	// 再生するアニメーション
	const std::map<SceneSelect::SelectScene, std::string> kPlayAnimName
	{
		{SceneSelect::SelectScene::kContinue, "continue"},
		{SceneSelect::SelectScene::kFirst, "first"},
		{SceneSelect::SelectScene::kOption, "option"},
		{SceneSelect::SelectScene::kGameEnd, "end"},
	};

	/*カメラ*/
	constexpr float kCameraHeight = 50.0f;		// カメラの注視点
	const VECTOR kCameraPos = VGet(0.0f, 70.0f, -170.0f);	// カメラ位置
	const VECTOR kCameraTarget = VGet(0.0f, 40.0f, 100.0f);	// カメラの視線方向

	/*カーソル*/
	const std::string kCursorId = "cursor_select";				// カーソルのID
	const std::string kEndSelectCursorId = "cursor_endSelect";	// ゲーム終了選択肢カーソルのID
	constexpr float kCursorInterval = 115.0f;					// カーソルの表示間隔
	constexpr float kEndSelectCursorInterval = 157.0f;			// ゲーム終了選択肢カーソル表示間隔
	
	/*UI*/
	const std::string kExplainId = "EXPLAIN_SELECT_";		// 選択中テキストの説明ID
	const std::string kEndSelectBgId= "bg_endSelect";		// ゲーム終了選択肢の背景ID
	const Vec2 kDispEndSelectPos = { 528.0f, 279.0f };		// ゲーム終了選択肢の表示位置
	const Vec2 kDispTextPos = { 156.0f, 210.0f };			// テキスト表示位置
	const Vec2 kDispExplainTextPos = { 100.0f, 830.0f };	// 説明テキスト表示位置
	const Vec2 kDispBgUnderPos = { 0.0f, 997.0f };			// 背景下部分表示位置
	const Vec2 kDispCopyrightPos = { 930.0f, 56.0f };		// 権利表記表示位置
	constexpr float kMoveCopyright = 80.0f;					// 権利表記の移動量
	const Vec2 kDispSavePos = { 259.0f, 115.0f };			// セーブ画面表示位置
	const Vec2 kDispSaveCursorPos = { 442.0f, 265.0f };		// セーブ画面のカーソル表示位置
	const Vec2 kDispDateSaveDataPos = { 730.0f, 305.0f };	  // 保存された現在時刻の表示位置
	const Vec2 kDispPlayTimeSaveDataPos = { 750.0f, 365.0f }; // 保存されたプレイ時間の表示位置

	/*フェード*/
	constexpr int kStartFadeAlpha = 255; // スタート時のフェード値
	constexpr int kFadeFrame = 8;		 // フェード変化量
}

SceneSelect::SceneSelect():
	m_copyrightPosX(Game::kScreenWidth),
	m_playAnimName(kPlayAnimName.at(SelectScene::kContinue)),
	m_isDispSaveData(false)
{
	m_fadeAlpha = kStartFadeAlpha;

	Sound::GetInstance().PlayBgm(SoundName::kBgm_select);

	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<PlayerSelect>();

	// カメラ位置設定
	SetCameraPositionAndTarget_UpVecY(kCameraPos, kCameraTarget);

	// モデル読み込み
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}

}

SceneSelect::~SceneSelect()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void SceneSelect::Init()
{
	m_pPlayer->Init();
	ChangeAnim(); 	// アニメーション設定
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	FadeOut(kFadeFrame); // フェードアウト
	if (m_isFadeOut) return shared_from_this(); // フェードアウト中は操作できないようにする

	// プレイヤー更新
	m_pPlayer->Update();

	// セーブデータ選択中
	if (m_isDispSaveData)
	{
		// 選択状態を更新
		UpdateSaveSelect(input, SaveData::SelectSaveData::kSaveNum);

		if (input.IsTriggered(InputId::kOk))
		{
			if (m_select == SelectScene::kContinue)
			{
				// 選択されたセーブデータを読み込む
				SaveData::GetInstance().Load(m_saveSelect);
			}
			if (m_select == SelectScene::kFirst)
			{
				// 新しくセーブデータを作成する
				SaveData::GetInstance().CreateNewData(m_saveSelect);
			}
			SceneChangeSound(SoundName::kBgm_select);
			FadeIn(kFadeFrame); // フェードイン

			return std::make_shared<SceneMain>();
		}

		if (input.IsTriggered(InputId::kBack))
		{
			SoundCancelSe();
			m_saveSelect = SaveData::SelectSaveData::one;
			m_isDispSaveData = false;
			return shared_from_this();
		}
	}
	// 終了選択肢
	else if (m_isChoice)
	{
		// 選択状態を更新
		UpdateChoice(input);
		m_pUi->UpdateCursor(kEndSelectCursorId);

		if (input.IsTriggered(InputId::kOk))
		{
			if (m_choiceSelect == Choice::kYes)
			{
				// ゲームを終了する
				SceneChangeSound(SoundName::kBgm_select);
				EndGame();
			}
			else if(m_choiceSelect == Choice::kNo)
			{
				// 選択肢を閉じる
				m_isChoice = false;
				m_choiceSelect = Choice::kYes;
				return shared_from_this();
			}
		}
	}
	else
	{
		// 選択状態を更新
		UpdateSelect(input, SelectScene::kSelectNum);
		m_pUi->UpdateCursor(kCursorId);

		// アニメーションを更新
		if (input.IsTriggered(InputId::kDown) || input.IsTriggered(InputId::kUp))
		{
			ChangeAnim();
		}

		// 権利表記の位置調整
		if (m_select == SelectScene::kCopyright)
		{
			m_copyrightPosX -= kMoveCopyright;
			m_copyrightPosX = std::max(kDispCopyrightPos.x, m_copyrightPosX);
		}
		else
		{
			m_copyrightPosX = Game::kScreenWidth;
		}
	}

	// 遷移
	if (input.IsTriggered(InputId::kOk))
	{
		SoundSelectSe();

		if (m_select == SelectScene::kContinue || m_select == SelectScene::kFirst)
		{
			m_isDispSaveData = true;
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == SelectScene::kGameEnd)
		{
			m_isChoice = true;
		}
	}
	else if (input.IsTriggered(InputId::kBack))
	{
		// 選択肢表示中は飛ばす
		if (m_isChoice) return shared_from_this();

		SceneChangeSound(SoundName::kBgm_select);
		// タイトル画面に戻る
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneSelect::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kSelectBg], true);	 // 背景表示

	if (m_select == SelectScene::kCopyright)
	{
		DrawCopyright(); // 権利表記を表示する
	}
	else
	{
		m_pPlayer->Draw(); // プレイヤー表示
	}

	DrawGraphF(kDispBgUnderPos.x, kDispBgUnderPos.y, m_handle[Handle::kSelectBgUnder], true); // 背景下部分表示

	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval); // カーソル表示
	DrawGraphF(kDispTextPos.x, kDispTextPos.y, m_handle[Handle::kSelectText], true); // テキスト表示
	DrawExplain();

	// セーブデータの表示
	if (m_isDispSaveData)
	{
		DrawSaveData();
	}

	// ゲーム終了の選択肢を表示
	if (m_isChoice)
	{
		m_pUi->DrawChoiceBg(kEndSelectBgId); // 背景表示
		m_pUi->DrawCursor(kEndSelectCursorId, m_choiceSelect, kEndSelectCursorInterval); // カーソル表示
		DrawGraphF(kDispEndSelectPos.x, kDispEndSelectPos.y, m_handle[Handle::kEndSelect], true); // テキスト表示
	}

	DrawFade();	// フェードインアウト描画

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif
}

void SceneSelect::ChangeAnim()
{
	if (m_select == SelectScene::kContinue)
	{
		m_playAnimName = kPlayAnimName.at(SelectScene::kContinue);
	}
	else if (m_select == SelectScene::kFirst)
	{
		m_playAnimName = kPlayAnimName.at(SelectScene::kFirst);
	}
	else if (m_select == SelectScene::kOption)
	{
		m_playAnimName = kPlayAnimName.at(SelectScene::kOption);
	}
	else if (m_select == SelectScene::kGameEnd)
	{
		m_playAnimName = kPlayAnimName.at(SelectScene::kGameEnd);
	}

	// プレイヤー位置更新
	m_pPlayer->ChangePos(m_select);
	// アニメーション再生
	m_pPlayer->ChangeAnim(m_playAnimName);
}

void SceneSelect::DrawSaveData()
{
	DrawGraphF(kDispSavePos.x, kDispSavePos.y, m_handle[Handle::kSaveBg], true);
	m_pUi->DrawSaveCursor(kDispSaveCursorPos, m_saveSelect);

	// セーブデータの情報を表示
	SaveData::GetInstance().DrawDateSaveData(kDispDateSaveDataPos);			// 現在時刻
	SaveData::GetInstance().DrawPlayTimeSaveData(kDispPlayTimeSaveDataPos);	// プレイ時間
}

void SceneSelect::DrawCopyright()
{
	DrawGraphF(m_copyrightPosX, kDispCopyrightPos.y, m_handle[Handle::kCopyright], true);

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_COPYRIGHT"); // シーン名表示
#endif
}

void SceneSelect::DrawExplain()
{
	// 現在選択中のテキストの説明を表示する
	std::string explainId = kExplainId + std::to_string(m_select);
	std::string drawExplain = LoadCsv::GetInstance().Get_cMessage(explainId);

	DrawFormatStringFToHandle(kDispExplainTextPos.x, kDispExplainTextPos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelect_explain)], drawExplain.c_str());
}