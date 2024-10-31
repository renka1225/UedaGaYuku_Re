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
	SceneOption(std::shared_ptr<SceneBase> pScene);

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

private:
	enum Select
	{
		kSound,		// サウンド
		kWindow,	// 画面サイズ
		kCredit,	// クレジット
		kSelectNum	// 選択数
	};

	std::shared_ptr<SceneBase> m_pPrevScene;	// 前に実行していたシーン
};

