#pragma once
#include "SceneBase.h"

/// <summary>
/// ゲームクリア
/// </summary>
class SceneClear : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneClear();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneClear();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

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

private:
	int m_clearBgModel;	// クリア背景の3Dモデル
};

