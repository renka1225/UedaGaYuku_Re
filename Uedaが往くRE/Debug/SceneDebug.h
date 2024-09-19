#pragma once
#include "SceneBase.h"

/// <summary>
/// デバッグ用シーン
/// </summary>
class SceneDebug : public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {};
	virtual void Init() {};
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	// シーンの種類
	enum SelectScene
	{
		kTitle,			// タイトル
		kSelect,		// セレクト画面
		kStage,			// ステージ
		kDebug,			// デバッグ用ステージ
		kOption,		// オプション
		kEnding,		// エンディング
		kSelectNum		// シーンの数
	};
};

