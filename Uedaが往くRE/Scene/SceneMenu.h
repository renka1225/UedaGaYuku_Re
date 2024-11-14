#pragma once
#include "SceneBase.h"
#include <vector>

class Player;

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
	SceneMenu(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer);

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

private:
	// 選択項目
	enum Select
	{
		kItem,			// アイテム使用
		kEnhance,		// 強化
		kOption,		// オプション
		kSave,			// セーブする
		kBackTitle,		// タイトルに戻る
		kSelectNum		// 選択数
	};

	// ハンドル
	std::vector<int> m_handle;
};

