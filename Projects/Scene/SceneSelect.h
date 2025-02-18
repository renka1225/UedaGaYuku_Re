#pragma once
#include "SceneBase.h"

class Camera;
class PlayerSelect;

/// <summary>
/// セレクト画面
/// </summary>
class SceneSelect : public SceneBase
{
public:
	// シーンの種類
	enum SelectScene
	{
		kContinue,	// 続きから
		kFirst,		// 初めから
		kOption,	// オプション
		kCopyright,	// 権利表記
		kGameEnd,	// ゲーム終了
		kSelectNum,	// 選択数
	};

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
	virtual void Init() override;

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
	/// アニメーション更新
	/// </summary>
	void ChangeAnim();

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
	std::shared_ptr<Camera> m_pCamera;			// カメラのポインタ
	std::shared_ptr<PlayerSelect> m_pPlayer;	// カメラのポインタ

	float m_copyrightPosX;			// 権利表記のX座標の位置
	std::string m_playAnimName;		// 再生するアニメーション名

	bool m_isDispSaveData;	// セーブデータを表示中かどうか(true:表示中)
};

