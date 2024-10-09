#pragma once
#include "SceneBase.h"

/// <summary>
/// オプション
/// </summary>
class SceneOption : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneOption() {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneOption() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override {}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;
};

