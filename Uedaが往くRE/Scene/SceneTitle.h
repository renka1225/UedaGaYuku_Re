#pragma once
#include "SceneBase.h"

/// <summary>
/// ƒ^ƒCƒgƒ‹
/// </summary>
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
};

