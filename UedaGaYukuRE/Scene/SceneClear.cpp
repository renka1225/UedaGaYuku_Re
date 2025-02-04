#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "UiBase.h"
#include "SceneTitle.h"
#include "SceneClear.h"

namespace
{
	// 選択項目
	enum Select
	{
		kTitle,		// タイトルに戻る
		kGameEnd,	// ゲーム終了
		kSelectNum	// 選択数
	};

	// 画像の種類
	enum Handle
	{
		kClear,
		kText,
		kEndSelect,
		kNum
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/clear/clear.png",
		"data/ui/clear/text.png",
		"data/ui/select/endSelect.png"
	};

	const Vec2 kGameoverPos = { 470.0f, 150.0f }; // クリア表示位置
	const Vec2 kTextPos = { 743.0f, 683.0f };	  // テキスト表示位置
	constexpr int kBgColor = 0xb3b3b3; // 背景色

	/*カーソル*/
	const std::string kCursorId = "cursor_clear"; // カーソルのID
	constexpr float kCursorInterval = 195.0f;	  // カーソルの表示間隔

	const std::string kEndSelectCursorId = "cursor_endSelect";	// ゲーム終了選択肢カーソルのID
	constexpr float kEndSelectCursorInterval = 157.0f;			// ゲーム終了選択肢カーソル表示間隔
	const std::string kEndSelectBgId = "bg_endSelect";			// ゲーム終了選択肢の背景ID
	const Vec2 kDispEndSelectPos = { 528.0f, 279.0f };			// ゲーム終了選択肢の表示位置

	/*フェード*/
	constexpr int kStartFadeAlpha = 255; // スタート時のフェード値
	constexpr int kFadeFrame = 8;		 // フェード変化量

	/*モデル*/
	const VECTOR kCaseModelPos = VGet(0.0f, -45.0f, 0.0f);	// モデル位置
	const VECTOR kCaseModelScale = VGet(0.8f, 0.8f, 0.8f);	// モデル拡大率
	const VECTOR kCaseModelAngle = VGet(0.0f, 0.0f, 0.0f);	// モデル角度
	const VECTOR kCameraPos = VGet(0.0f, 50.0f, -120.0f);	// カメラの位置
	const VECTOR kCameraTarget = VGet(0.0f, 0.0f, 0.0f);	// カメラの注視点

}

SceneClear::SceneClear():
	m_clearBgModel(-1)
{
	Sound::GetInstance().PlayBgm(SoundName::kBgm_clear);

	m_fadeAlpha = kStartFadeAlpha;

	m_select = Select::kTitle;

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}

	m_clearBgModel = MV1LoadModel("data/model/clearBg.mv1");
}

SceneClear::~SceneClear()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}

	MV1DeleteModel(m_clearBgModel);
}

void SceneClear::Init()
{
	// 3Dモデル更新
	MV1SetPosition(m_clearBgModel, kCaseModelPos);
	MV1SetScale(m_clearBgModel, kCaseModelScale);
	MV1SetRotationXYZ(m_clearBgModel, kCaseModelAngle);
	// カメラ位置更新
	SetCameraPositionAndTarget_UpVecY(VAdd(kCaseModelPos, kCameraPos), kCameraTarget);
}

std::shared_ptr<SceneBase> SceneClear::Update(Input& input)
{
	FadeOut(kFadeFrame); // フェードアウト
	if (m_isFadeOut) return shared_from_this(); // フェードアウト中は操作できないようにする

	// 終了選択肢
	if (m_isChoice)
	{
		// 選択状態を更新
		UpdateChoice(input);
		m_pUi->UpdateCursor(kEndSelectCursorId);

		if (input.IsTriggered(InputId::kOk))
		{
			if (m_choiceSelect == Choice::kYes)
			{
				EndGame();
			}
			else if (m_choiceSelect == Choice::kNo)
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
		UpdateSelect(input, Select::kSelectNum);
		m_pUi->UpdateCursor(kCursorId);
	}

	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == Select::kTitle)
		{
			FadeIn(kFadeFrame); // フェードイン
			SceneChangeSound(SoundName::kBgm_clear);
			return std::make_shared<SceneTitle>();
		}
		else if (m_select == Select::kGameEnd)
		{
			m_isChoice = true;
		}
	}

	return shared_from_this();
}

void SceneClear::Draw()
{
	// 背景表示
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBgColor, true);

	MV1DrawModel(m_clearBgModel); // 3Dモデル表示

	// クリアテキスト表示
	DrawGraphF(kGameoverPos.x, kGameoverPos.y, m_handle[Handle::kClear], true);

	m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval);

	DrawGraphF(kTextPos.x, kTextPos.y, m_handle[Handle::kText], true);

	DrawFade();	// フェードインアウト描画

	// ゲーム終了の選択肢を表示
	if (m_isChoice)
	{
		m_pUi->DrawChoiceBg(kEndSelectBgId); // 背景表示
		m_pUi->DrawCursor(kEndSelectCursorId, m_choiceSelect, kEndSelectCursorInterval); // カーソル表示
		DrawGraphF(kDispEndSelectPos.x, kDispEndSelectPos.y, m_handle[Handle::kEndSelect], true); // テキスト表示
	}

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_CLEAR");
#endif
}