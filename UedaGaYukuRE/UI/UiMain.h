#pragma once
#include "Player.h"
#include "UiBase.h"

// 会話用のID
namespace ConversationID
{
	const std::string kTalkStart = "MSG_TALKSTART";
	const std::string kSelect = "MSG_SELECT_";
	const std::string kSelect1 = kSelect + "1";
	const std::string kSelect2 = kSelect + "2";
	const std::string kSelect3 = kSelect + "3";
	const std::string kSelect4 = kSelect + "4";
	const std::string kSelect5 = kSelect + "5";
	const std::string kBattleOk = "MSG_BATTLE_OK";
	const std::string kBattleNg = "MSG_BATTLE_NG";
	const std::string kDeadNum = "MSG_DEADNUM";
	const std::string kRecoveryOk = "MSG_RECOVERY_OK";
	const std::string kRecoveryNg = "MSG_RECOVERY_NG";
	const std::string kRecoveryMax = "MSG_RECOVERY_MAX";
	const std::string kGetItem = "MSG_GETITEM";
}

/// <summary>
/// SceneMainで使うUI
/// </summary>
class UiMain : public UiBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiMain();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UiMain();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// ロード中の演出を表示
	/// </summary>
	void DrawLoading();

	/// <summary>
	/// バトル開始時の演出を表示
	/// </summary>
	void DrawBattleStart();

	/// <summary>
	/// バトル終了時の演出を表示
	/// </summary>
	/// <param name="time">現在の演出時間</param>
	void DrawBattleEnd(int time);

	/// <summary>
	/// バトルに負けた場合の演出を表示
	/// </summary>
	void DrawBattleLose();

	/// <summary>
	/// エンディングの演出を表示
	/// </summary>
	void DrawEnding();

	/// <summary>
	/// NPC用のUIを表示する
	/// </summary>
	/// <param name="pos">NPCの座標</param>
	void DrawNpcUi(VECTOR pos);

	/// <summary>
	/// バトル中のUIを表示
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void DrawBattleUi(const Player& pPlayer);

	/// <summary>
	/// 所持金UIのアニメーションをセットする
	/// </summary>
	void SetAnimMoneyUi();

	/// <summary>
	/// 所持金のUIを表示
	/// </summary>
	void DrawMoneyUi();

	/// <summary>
	/// ミニマップ表示
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="pEnemy">敵参照</param>
	void DrawMiniMap(const Player& pPlayer, std::vector<std::shared_ptr<EnemyBase>> pEnemy);

	/// <summary>
	/// 操作説明表示
	/// </summary>
	/// <param name="isBattle">バトル中かどうか</param>
	void DrawOperation(bool isBattle);

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

	/// <summary>
	/// チュートリアル表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTutorial(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアル1表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTuto1(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアル2表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTuto2(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアル3表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTuto3(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアル4表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTuto4(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアル4表示
	/// </summary>
	/// <param name="tutoInfo">チュートリアル情報</param>
	void DrawTuto5(Player::TutorialInfo tutoInfo);

	/// <summary>
	/// チュートリアルのチェックを表示
	/// </summary>
	/// <param name="posY">Y座標の表示位置</param>
	void DrawTutorialCheck(float posY);

	/// <summary>
	/// チュートリアル完了時の表示
	/// </summary>
	void DrawTutorialOk();

	/// <summary>
	/// チュートリアルの現在の実行回数を表示
	/// </summary>
	/// <param name="pos">表示位置</param>
	/// <param name="currentNum">現在の実行回数</param>
	void DrawTutorialCurrentNum(Vec2 pos, int currentNum);

private:
	float m_loadingAnimTime;	// ローディング中のアニメーション時間

	Vec2 m_dispMoneyPos;		// 所持金の表示位置
	int m_dispMoneyAnimTime;	// 所持金のアニメーションを行う時間

	float m_dispGekihaTextScale;	// "撃破"テキストの表示サイズ
	float m_dispEnemyKindScale;		// バトル開始時の敵種類の表示サイズ
	float m_dispNowBattlePosX;		// バトル中のUI表示位置X

	std::string m_currentTalkId;	// 現在表示している会話ID
	std::string m_currentTalkText;	// 現在表示している会話テキスト
	bool m_isTalkTextComplete;		// 全文表示したかどうか
	int m_currentTalkTextCount;		// 現在表示中の文字数
	int m_currentFrame;				// テキストを表示してからの経過フレーム数
};

