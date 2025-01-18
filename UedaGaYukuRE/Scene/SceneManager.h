#pragma once
#include <memory>

class SceneBase;
class Input;

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneManager() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	void Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	std::shared_ptr<SceneBase> m_pScene; // シーンのポインタ

	int m_playTime;
};

