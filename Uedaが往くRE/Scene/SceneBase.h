#pragma once
#include <memory>

class Input;

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase();
	virtual void Init() = 0;
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw() = 0;

protected:
	void UpdateSelect(Input& input, int selectNum);	// 選択状態を更新

protected:
	int m_select;	// 現在の選択状態
};

