#pragma once
#include "SceneBase.h"

class Player;

/// <summary>
/// アイテム使用画面
/// </summary>
class SceneUseItem : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneUseItem(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneUseItem();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}

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

private:
	/// <summary>
	/// アイテムを使用した場合
	/// </summary>
	void UseItem();

	/// <summary>
	/// カーソル移動の処理
	/// </summary>
	void MoveCursor(Input& input);

	/// <summary>
	/// カーソルを表示
	/// </summary>
	void DrawCursor();

private:
	std::vector<int> m_possessItem;	// プレイヤーが所持しているアイテム
	int m_time; // アイテム画面を開いてからの時間
};

