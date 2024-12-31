#pragma once
#include "SceneBase.h"

/// <summary>
/// ゲームオーバー
/// </summary>
class SceneGameover : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGameover();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneGameover();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
};

