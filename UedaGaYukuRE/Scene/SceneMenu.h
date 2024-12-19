#pragma once
#include "SceneBase.h"
#include <vector>

class Player;
class Camera;

/// <summary>
/// メニュー画面
/// </summary>
class SceneMenu : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pScene">前に実行していたシーン</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	/// <param name="pPlayer">カメラのポインタ</param>
	SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneMenu();

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

