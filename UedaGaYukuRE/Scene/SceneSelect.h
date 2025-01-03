#pragma once
#include "SceneBase.h"

/// <summary>
/// セレクト画面
/// </summary>
class SceneSelect : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override {}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Draw() override;

private:
	/// <summary>
	/// セーブデータの表示
	/// </summary>
	void DrawSaveData();

	/// <summary>
	/// 権利表記の表示
	/// </summary>
	void DrawCopyright();

	/// <summary>
	/// 選択中テキストの説明
	/// </summary>
	void DrawExplain();

private:
	bool m_isDispSaveData;	// セーブデータを表示中かどうか(true:表示中)
	bool m_isDispCopyright; // 権利表記の表示中かどうか(true:表示中)
};

