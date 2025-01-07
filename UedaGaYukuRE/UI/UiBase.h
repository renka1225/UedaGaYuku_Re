#pragma once
#include "Vec2.h"
#include <vector>
#include <memory>
#include <string>

class Player;
class EnemyBase;

/// <summary>
/// UI用の基底クラス
/// </summary>
class UiBase
{
public:
	// UI表示情報
	struct UiData
	{
		float LTposX;		// 左上X座標
		float LTposY;		// 左上Y座標
		float RBposX;		// 右下X座標
		float RBposY;		// 右下Y座標
		float width;		// 横幅
		float height;		// 縦幅
		float scale;		// 拡大率
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UiBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// カーソルの表示を更新
	/// </summary>
	/// <param name="cursorId">表示するカーソルのID</param>
	void UpdateCursor(std::string cursorId);

	/// <summary>
	///  カーソル表示
	/// </summary>
	/// <param name="cursorId">表示するカーソルのID</param>
	/// <param name="select">選択状態</param>
	/// <param name="interval">表示間隔</param>
	void DrawCursor(std::string cursorId, int select, float interval);

	/// <summary>
	/// セーブ用のカーソルを表示
	/// </summary>
	/// <param name="pos">表示位置</param>
	/// <param name="select">選択状態</param>
	void DrawSaveCursor(Vec2 pos, int select);

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
	void DrawBattleEnd();

	/// <summary>
	/// エンディングの演出を表示
	/// </summary>
	void DrawEnding();

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

protected:
	std::map<std::string, UiData> m_uiData;	// UIのデータ
	std::vector<int> m_handle;	// ハンドル

	float m_cursorWidth;		// カーソルの横幅
	int m_cursorDisplayTime;	// カーソルを表示する時間
	int m_cursorAlpha;			// カーソルのα値

	float m_loadingAnimTime;	// ローディング中のアニメーション時間

	float m_dispEnemyKindScale;	// バトル開始時の敵種類の表示サイズ
	float m_dispNowBattlePosX;	// バトル中のUI表示位置X

};

