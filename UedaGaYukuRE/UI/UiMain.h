#pragma once
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
	/// ミニマップ表示
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="pEnemy">敵参照</param>
	void DrawMiniMap(const Player& pPlayer, std::vector<std::shared_ptr<EnemyBase>> pEnemy);

	/// <summary>
	/// 操作説明表示
	/// </summary>
	void DrawOperation();

	/// <summary>
	/// 会話画面を表示
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="id">テキストID</param>
	/// <param name="clearNum">条件の敵数</param>
	void DrawTalk(const Player& pPlayer, std::string id, int clearNum);

private:
	float m_loadingAnimTime;	// ローディング中のアニメーション時間

	float m_dispGekihaTextScale;	// "撃破"テキストの表示サイズ
	float m_dispEnemyKindScale;		// バトル開始時の敵種類の表示サイズ
	float m_dispNowBattlePosX;		// バトル中のUI表示位置X
};

