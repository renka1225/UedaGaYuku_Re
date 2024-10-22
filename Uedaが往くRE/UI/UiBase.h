#pragma once
#include "Vec2.h"
#include <memory>

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
		float posX;		// X座標
		float posY;		// Y座標
		float scale;	// 拡大率
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

protected:
	std::shared_ptr<Player> m_pPlayer; // プレイヤーのポインタ
	UiData m_uiData;		// UIのデータ
	int m_minimapHandle;	// ミニマップの画像
};

