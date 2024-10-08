#pragma once
#include "EnemyBase.h"

/// <summary>
/// 敵ID01のクラス
/// </summary>
class Enemy01 : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy01();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy01();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stage">ステージ参照</param>
	virtual void Update(Stage& stage) override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;
};

