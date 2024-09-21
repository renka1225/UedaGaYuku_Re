#pragma once
#include "SceneBase.h"

/// <summary>
/// ƒIƒvƒVƒ‡ƒ“
/// </summary>
class SceneOption : public SceneBase
{
public:
	SceneOption();
	virtual ~SceneOption();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
};

