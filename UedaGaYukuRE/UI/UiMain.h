#pragma once
#include "Player.h"
#include "UiBase.h"

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
	/// ロード中の演出を更新
	/// </summary>
	void UpdateLoading(const Input& input);

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
	/// <param name="money">現在の所持金額</param>
	void DrawMoneyUi(int money);

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

	/// <summary>
	/// チュートリアルの心得表示
	/// </summary>
	/// <param name="currentTutoNum">現在のチュートリアル数</param>
	void DrawTutoKnowledge(int currentTutoNum);

private:
	std::vector<int> m_introduceHandle;	// 紹介テキストの画像
	std::vector<int> m_knowledgeHandle;	// 心得の画像
	int m_nowLoadingIntroduce;			// 現在表示している紹介画像
	Vec2 m_dispIntroducePos;	// 紹介テキストの表示位置
	float m_loadingTime;		// ローディングが開始してからの時間
	float m_loadingAnimTime;	// ローディング中のアニメーション時間

	Vec2 m_dispMoneyPos;		// 所持金の表示位置
	int m_dispMoneyAnimTime;	// 所持金のアニメーションを行う時間

	float m_dispGekihaTextScale;	// "撃破"テキストの表示サイズ
	float m_dispEnemyKindScale;		// バトル開始時の敵種類の表示サイズ
	float m_dispNowBattlePosX;		// バトル中のUI表示位置X
};

