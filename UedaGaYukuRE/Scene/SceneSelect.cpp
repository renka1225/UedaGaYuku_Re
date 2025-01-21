#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Font.h"
#include "Sound.h"
#include "LoadCsv.h"
#include "SaveData.h"
#include "Camera.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

namespace
{
	// シーンの種類
	enum SelectScene
	{
		kContinue,	// 続きから
		kFirst,		// 初めから
		kOption,	// オプション
		kCopyright,	// 権利表記
		kGameEnd,	// ゲーム終了
		kSelectNum,	// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kSelectBg,		// 背景
		kSaveBg,		// セーブ画面の背景
		kSelectText,	// テキスト
		kNum			// ハンドルの数
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/select/bg.png",
		"data/ui/select/save.png",
		"data/ui/select/text.png"
	};

	// 再生するアニメーション
	const std::map<SelectScene, std::string> kPlayAnimName
	{
		{SelectScene::kContinue, "continue"},
		{SelectScene::kFirst, "first"},
		{SelectScene::kOption, "option"},
		{SelectScene::kGameEnd, "end"},
	};

	constexpr float kCameraHeight = 30.0f;		// カメラの高さ
	const VECTOR kCameraPos = VGet(0.0f, 70.0f, -200.0f); // カメラの位置
	const VECTOR kPlayerPos = VGet(100.0f, 0.0f, 0.0f); // プレイヤーの位置

	const char* const kPlayerModelPath = "data/model/chara/player_select.mv1"; // プレイヤーのモデルパス
	constexpr float kPlayerModelScale = 0.6f;		// モデルの拡大率

	const std::string kCursorId = "cursor_select";	// カーソルのID
	constexpr float kCursorInterval = 115.0f;		// カーソルの表示間隔
	
	const std::string kExplainId = "EXPLAIN_SELECT_";	 // 選択中テキストの説明ID
	const Vec2 kDispTextPos = { 156.0f, 210.0f };		 // テキスト表示位置
	const Vec2 kDispExplainTextPos = { 100.0f, 830.0f }; // 説明テキスト表示位置

	const Vec2 kDispSavePos = { 259.0f, 115.0f };		 // セーブ画面表示位置
	const Vec2 kDispSaveCursorPos = { 442.0f, 265.0f };  // セーブ画面のカーソル表示位置
	const Vec2 kDispDateSaveDataPos = { 730.0f, 305.0f };	  // 保存された現在時刻の表示位置
	const Vec2 kDispPlayTimeSaveDataPos = { 750.0f, 365.0f }; // 保存されたプレイ時間の表示位置

	/*フェード*/
	constexpr int kStartFadeAlpha = 255; // スタート時のフェード値
	constexpr int kFadeFrame = 8;		 // フェード変化量
}

SceneSelect::SceneSelect():
	m_playAnimName(""),
	m_isDispSaveData(false)
{
	m_fadeAlpha = kStartFadeAlpha;

	Sound::GetInstance().PlayBgm(SoundName::kBgm_select);

	// カメラ
	m_pCamera = std::make_shared<Camera>();

	// カメラの注視点を設定する
	VECTOR target = VAdd(kPlayerPos, VGet(0.0f, kCameraHeight, 0.0f));
	m_pCamera->SetCameraInfo(kCameraPos, target);

	// モデル読み込み
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}

	m_playerModelHandle = MV1LoadModel(kPlayerModelPath);
	MV1SetScale(m_playerModelHandle, VGet(kPlayerModelScale, kPlayerModelScale, kPlayerModelScale));
	MV1SetPosition(m_playerModelHandle, kPlayerPos);
}

SceneSelect::~SceneSelect()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
	MV1DeleteModel(m_playerModelHandle);
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	FadeOut(kFadeFrame); // フェードアウト
	if (m_isFadeOut) return shared_from_this(); // フェードアウト中は操作できないようにする

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
	else
	{
		UpdateSelect(input, SelectScene::kSelectNum);
		m_pUi->UpdateCursor(kCursorId);
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
			SceneChangeSound(SoundName::kBgm_select);

			// ゲームを終了する
			EndGame();
		}
	}
	else if (input.IsTriggered(InputId::kBack))
	{
		SceneChangeSound(SoundName::kBgm_select);
		// タイトル画面に戻る
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneSelect::Draw()
{
	DrawGraph(0, 0, m_handle[Handle::kSelectBg], true);		 // 背景表示
	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval); // カーソル表示
	DrawGraphF(kDispTextPos.x, kDispTextPos.y, m_handle[Handle::kSelectText], true); // テキスト表示
	DrawExplain();

	// セーブデータの表示
	if (m_isDispSaveData)
	{
		DrawSaveData();
	}

	DrawFade();	// フェードインアウト描画

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif

	// 権利表記を表示する
	if (m_select == SelectScene::kCopyright)
	{
		DrawCopyright();
	}
	else
	{
		MV1DrawModel(m_playerModelHandle); // プレイヤー表示
	}

	printfDx("X:%.2f,Y:%.2f,Z:%.2f\n", m_pCamera->GetPos().x, m_pCamera->GetPos().y, m_pCamera->GetPos().z);
}

void SceneSelect::UpdateAnim()
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

	// アニメーション再生
	int animIndex = MV1GetAnimIndex(m_playerModelHandle, m_playAnimName.c_str());
	MV1AttachAnim(m_playerModelHandle, animIndex);

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
	DrawBoxAA(930.0f, 56.0f, 1860.0f, 765.0f, 0x000000, true);

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