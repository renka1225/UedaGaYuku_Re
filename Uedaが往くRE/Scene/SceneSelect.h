#pragma once
#include "SceneBase.h"

/// <summary>
/// セレクト画面
/// </summary>
class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	virtual ~SceneSelect();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	// シーンの種類
	enum SelectScene
	{
		kMain,		// メイン
		kOption,	// オプション
		kSelectNum,	// 選択数
	};
};

