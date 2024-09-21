#pragma once
#include "SceneBase.h"

/// <summary>
/// ƒZƒŒƒNƒg‰æ–Ê
/// </summary>
class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	virtual ~SceneSelect();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
};

