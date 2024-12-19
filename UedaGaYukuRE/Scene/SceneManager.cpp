#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include "Input.h"

SceneManager::SceneManager():
	m_pScene(nullptr)
{
}

void SceneManager::Init()
{
#ifdef _DEBUG
	m_pScene = std::make_shared<SceneDebug>();
	m_pScene->Init();
#else
	m_pScene = std::make_shared<SceneTitle>();
	m_pScene->Init();
#endif
}

void SceneManager::Update(Input& input)
{
	std::shared_ptr<SceneBase> pNext = m_pScene->Update(input);

	// 遷移先のシーンの開始処理を行う
	if (pNext != m_pScene)
	{
		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}
}

void SceneManager::Draw()
{
	m_pScene->Draw();
}
