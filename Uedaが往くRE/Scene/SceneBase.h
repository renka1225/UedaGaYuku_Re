#pragma once
#include <memory>

class Input;
class UiBase;

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

#ifdef _DEBUG
	/// <summary>
	/// 現在のシーンをデバッグ表示する
	/// </summary>
	/// <param name="sceneName">現在のシーン名</param>
	void DrawSceneText(const char* sceneName);
#endif

protected:
	/// <summary>
	///	選択状態を更新 
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="selectNum">選択肢の数</param>
	void UpdateSelect(Input& input, int selectNum);

protected:
	std::shared_ptr<UiBase> m_pUi; // UIのポインタ
	int m_select;	// 現在の選択状態
};

