#pragma once
#include "Vec2.h"
#include <memory>
#include <string>

class Player;

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
		float scale;		// 拡大率
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiBase(std::shared_ptr<Player> pPlayer);

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
	virtual void Draw();

	/// <summary>
	///  カーソル表示
	/// </summary>
	/// <param name="pos">表示位置</param>
	/// <param name="select">選択状態</param>
	/// <param name="interval">表示間隔</param>
	/// <param name="sceneName">現在のシーン</param>
	void DrawCursor(Vec2 pos, int select, float interval, std::string sceneName);

protected:
	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ
	UiData m_uiData;					// UIのデータ
	int m_minimapHandle;				// ミニマップの画像
};

