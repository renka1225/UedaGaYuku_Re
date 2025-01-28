#include "DxLib.h"
#include "Game.h"
#include "Font.h"
#include "LoadCsv.h"
#include "Player.h"
#include "SceneMain.h"
#include "UiConversation.h"

namespace
{
	/*画像の種類*/
	enum Handle
	{
		kTextBox,			// テキストボックス
		kTalkSelectBg,		// 会話選択肢の背景
		kNum				// 画像の種類
	};

	/*画像のパス*/
	const char* kHandle[Handle::kNum]
	{
		"data/ui/main/textBox.png",
		"data/ui/main/talkSelectBg.png",
	};

	/*選択肢表示位置*/
	const std::map<int, Vec2> kTalkSelectTextPos
	{
		{SceneMain::TalkSelect::kBattle, {773.0f, 198.0f}},
		{SceneMain::TalkSelect::kDeadEnemyNum, {706.0f, 296.0f}},
		{SceneMain::TalkSelect::kRecovery, {846.0f, 393.0f}},
		{SceneMain::TalkSelect::kGetItem, {771.0f, 490.0f}},
		{SceneMain::TalkSelect::kBack, {868.0f, 585.0f}},
	};

	const Vec2 kTextBoxPos = { 116.0f, 766.0f };		// テキストボックス位置
	const Vec2 kTalkNamePos = { 287.0f, 775.0f };		// 名前表示位置
	const Vec2 kTalkPos = { 489.0f, 850.0f };			// テキスト表示位置
	const Vec2 kTalkSelectBgPos = { 423.0f, 106.0f };	// 選択肢の背景位置
	constexpr int kDrawCountInterval = 3;				// 新たな文字を表示するフレーム数

}

UiConversation::UiConversation():
	m_currentTalkText(""),
	m_isTalkTextComplete(false),
	m_currentTalkTextCount(0),
	m_currentFrame(0)
{
	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandle[i]);
	}
}

UiConversation::~UiConversation()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void UiConversation::ResetDispTalk()
{
	m_currentTalkTextCount = 0;
	m_currentFrame = 0;
	m_isTalkTextComplete = false;
	m_currentTalkText.clear();
}

void UiConversation::UpdateDispTalk(std::string id)
{
	// 会話テキストの表示状態をリセット
	if (m_currentTalkText.empty())
	{
		ResetDispTalk();
	}

	// 会話IDが変わった場合
	if (id != m_currentTalkId)
	{
		ResetDispTalk();
		m_currentTalkId = id;

		// 新たな会話テキストを取得
		m_currentTalkText = LoadCsv::GetInstance().GetConversationText(m_currentTalkId);
	}

	// テキストを1文字ずつ表示する
	if (!m_isTalkTextComplete)
	{
		m_currentFrame++;

		// 全文字表示された場合
		if (m_currentTalkTextCount >= m_currentTalkText.size())
		{
			m_isTalkTextComplete = true;
			return;
		}

		// 表示テキストを1文字追加する
		bool isAdd = m_currentFrame % kDrawCountInterval == 0;
		if (isAdd)
		{
			m_currentTalkTextCount++;
		}
	}
}

void UiConversation::DrawTalk(const Player& pPlayer, int clearNum)
{
	// テキストボックス表示
	DrawGraphF(kTextBoxPos.x, kTextBoxPos.y, m_handle[Handle::kTextBox], true);

	// 名前表示
	std::string drawName = LoadCsv::GetInstance().GetConversationName(m_currentTalkId);
	DrawFormatStringFToHandle(kTalkNamePos.x, kTalkNamePos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTalk_Name)], drawName.c_str());

	// 敵数の表示
	int drawNum = 0;
	if (m_currentTalkId == ConversationID::kBattleNg)
	{
		drawNum = clearNum - pPlayer.GetDeadEnemyNum();
	}
	else if (m_currentTalkId == ConversationID::kDeadNum)
	{
		drawNum = pPlayer.GetDeadEnemyNum();
	}

	// 現在の文字数までのテキストを表示する
	std::string drawText = m_currentTalkText.substr(0, m_currentTalkTextCount);

	// テキスト表示
	DrawFormatStringFToHandle(kTalkPos.x, kTalkPos.y, Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTalk)], drawText.c_str(), drawNum);
}

void UiConversation::DrawTalkSelectBg()
{
	DrawGraphF(kTalkSelectBgPos.x, kTalkSelectBgPos.y, m_handle[Handle::kTalkSelectBg], true);
}

void UiConversation::DrawTalkSelectText()
{
	for (int i = 0; i < SceneMain::TalkSelect::kTalkNum; i++)
	{
		std::string drawText = LoadCsv::GetInstance().GetConversationText(ConversationID::kSelect + std::to_string((i + 1)));
		DrawStringFToHandle(kTalkSelectTextPos.at(i).x, kTalkSelectTextPos.at(i).y, drawText.c_str(), Color::kColorW, Font::m_fontHandle[static_cast<int>(Font::FontId::kTalk_select)]);
	}
}
