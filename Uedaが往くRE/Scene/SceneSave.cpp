#include "Input.h"
#include "Player.h"
#include "SceneSave.h"

SceneSave::SceneSave(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
}

std::shared_ptr<SceneBase> SceneSave::Update(Input& input)
{
	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // メニュー画面に戻る
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneSave::Draw()
{
#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_SAVE");
#endif
}
