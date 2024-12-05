#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "Font.h"
#include "Sound.h"
#include "LoadCsv.h"
#include "SaveData.h"
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
		kSelectText,	// テキスト
		kNum			// ハンドルの数
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/select/bg.png",
		"data/ui/select/text.png"
	};

	const std::string kCursorId = "cursor_select";	// カーソルのID
	constexpr float kCursorInterval = 115.0f;		// カーソルの表示間隔
	
	const std::string kExplainId = "EXPLAIN_SELECT_";	// 選択中テキストの説明ID
	const Vec2 kDispTextPos = { 156.0f, 210.0f };		// テキスト表示位置
	const Vec2 kDispExplainTextPos = { 100.0f, 830.0f }; // 説明テキスト表示位置
}

SceneSelect::SceneSelect():
	m_isDispCopyright(false)
{
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

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// 選択状態更新
	if (!m_isDispCopyright)
	{
		UpdateSelect(input, SelectScene::kSelectNum);
		m_pUi->UpdateCursor(kCursorId);
	}
	else
	{
		// 戻るボタンを押したら選択画面に戻る
		if (input.IsTriggered(InputId::kBack))
		{
			m_isDispCopyright = false;
			return shared_from_this();
		}
	}

	// 遷移
	if (input.IsTriggered(InputId::kOk))
	{
		if (m_select == SelectScene::kContinue)
		{
			// 選択されたセーブデータを読み込む
			SaveData::GetInstance().Load();
			SoundSelectSe();
			return std::make_shared<SceneMain>();
		}
		if (m_select == kFirst)
		{
			// 新しくセーブデータを作成する
			SaveData::GetInstance().CreateNewData();
			SoundSelectSe();
			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			SoundSelectSe();
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == SelectScene::kCopyright)
		{
			SoundSelectSe();
			m_isDispCopyright = true;
		}
		else if (m_select == SelectScene::kGameEnd)
		{
			SoundSelectSe();
			// ゲームを終了する
			EndGame();
		}
	}
	else if (input.IsTriggered(InputId::kBack))
	{
		// タイトル画面に戻る
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneSelect::Draw()
{
	// 権利表記を表示する
	if (m_isDispCopyright)
	{
		DrawCopyright();
	}
	else
	{
		DrawGraph(0, 0, m_handle[Handle::kSelectBg], true);		 // 背景表示
		m_pUi->DrawCursor(kCursorId, m_select, kCursorInterval); // カーソル表示
		DrawGraphF(kDispTextPos.x, kDispTextPos.y, m_handle[Handle::kSelectText], true); // テキスト表示
		DrawExplain();

#ifdef _DEBUG
		DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif
	}
}

void SceneSelect::DrawCopyright()
{
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
