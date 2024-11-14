#include "Input.h"
#include "SaveData.h"
#include "Camera.h"
#include "Player.h"
#include "SceneSave.h"

SceneSave::SceneSave(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
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
	// セーブデータを書き込む
	if (input.IsTriggered(InputId::kA))
	{
		SaveData::GetInstance().WriteData(*m_pPlayer, *m_pCamera);
	}

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
