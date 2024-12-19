#pragma once
#include "SceneBase.h"

/// <summary>
/// タイトル
/// </summary>
class SceneTitle : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTitle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;

	/// <summary>
	/// f描画
	/// </summary>
	virtual void Draw() override;

private:
	/// <summary>
	/// PRESSANYBUTTONのテキスト表示を更新する
	/// </summary>
	void UpdatePressText();

	/// <summary>
	/// ロゴの表示を更新する
	/// </summary>
	void UpdateLogo();

	/// <summary>
	/// サウンドの更新を行う
	/// </summary>
	void UpdateSound();

	/// <summary>
	/// シーン遷移のボタンを押したか
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>押したかどうか</returns>
	bool CheckInputSceneChange(Input& input);

private:
	int m_time;				// 経過時間
	int m_textDisplayTime;	// テキストを表示する時間
	float m_titleLogoScale;	// タイトルロゴの拡大率
	float m_titleLogoRot;	// タイトルロゴの回転率
	int m_textAlpha;		// テキストのアルファ値
};

