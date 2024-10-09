#pragma once
#include "SceneBase.h"

/// <summary>
/// セレクト画面
/// </summary>
class SceneSelect : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneSelect() {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneSelect() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override {}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Draw() override;

private:
	// シーンの種類
	enum SelectScene
	{
		kMain,		// メイン
		kOption,	// オプション
		kSelectNum,	// 選択数
	};
};

