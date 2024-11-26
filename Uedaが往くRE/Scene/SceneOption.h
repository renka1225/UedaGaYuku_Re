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
	virtual ~SceneOption();

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
	void UpdateSound(Input& input);		 // サウンド更新
	void UpdateWindowMode(Input& input); // 画面サイズの変更
	void DrawSound();					 // サウンド部分表示
	void DrawWindowMode();				 // 画面サイズ部分表示

private:

	std::shared_ptr<SceneBase> m_pPrevScene;	// 前に実行していたシーン

	int m_afterSelect;	// 選択後の状態
	bool m_isSound;		// サウンドが選択中か(true:選択中)
	bool m_isWindow;	// 画面サイズが選択中か(true:選択中)
};

