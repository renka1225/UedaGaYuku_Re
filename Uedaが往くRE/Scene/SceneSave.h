#pragma once
#include "SceneBase.h"

class Player;

/// <summary>
/// セーブ画面
/// </summary>
class SceneSave : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneSave(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneSave();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
};

