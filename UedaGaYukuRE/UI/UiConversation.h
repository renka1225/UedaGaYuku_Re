#pragma once
#include "UiBase.h"
#include <string>

class Player;

/// <summary>
/// 会話関連のUIを管理する
/// </summary>
class UiConversation : public UiBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiConversation();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UiConversation();

	/// <summary>
	/// 会話状態をリセットする
	/// </summary>
	void ResetDispTalk();

	/// <summary>
	/// 会話の表示状態を更新する
	/// <param name="id">現在のテキストID</param>
	/// </summary>
	void UpdateDispTalk(std::string id);

	/// <summary>
	/// 会話画面を表示
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="clearNum">条件の敵数</param>
	void DrawTalk(const Player& pPlayer, int clearNum);

	/// <summary>
	/// 会話の選択肢の背景を表示
	/// </summary>
	void DrawTalkSelectBg();

	/// <summary>
	/// 会話の選択肢を表示
	/// </summary>
	void DrawTalkSelectText();

private:
	std::string m_currentTalkId;	// 現在表示している会話ID
	std::string m_currentTalkText;	// 現在表示している会話テキスト
	bool m_isTalkTextComplete;		// 全文表示したかどうか
	int m_currentTalkTextCount;		// 現在表示中の文字数
	int m_currentFrame;				// テキストを表示してからの経過フレーム数
};

// 会話用のID
namespace ConversationID
{
	const std::string kTalkStart = "TALKSTART";
	const std::string kSelect = "SELECT_";
	const std::string kSelect1 = kSelect + "1";
	const std::string kSelect2 = kSelect + "2";
	const std::string kSelect3 = kSelect + "3";
	const std::string kSelect4 = kSelect + "4";
	const std::string kSelect5 = kSelect + "5";
	const std::string kBattleOk = "BATTLE_OK";
	const std::string kBattleNg = "BATTLE_NG";
	const std::string kDeadNum = "DEADNUM";
	const std::string kRecoveryOk = "RECOVERY_OK";
	const std::string kRecoveryNg = "RECOVERY_NG";
	const std::string kRecoveryMax = "RECOVERY_MAX";
	const std::string kGetItem = "GETITEM";
}