#include "Input.h"
#include "Player.h"
#include "SceneEnhance.h"

SceneEnhance::SceneEnhance(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
}

SceneEnhance::~SceneEnhance()
{
}

void SceneEnhance::Init()
{
}

std::shared_ptr<SceneBase> SceneEnhance::Update(Input& input)
{
	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // メニュー画面に戻る
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneEnhance::Draw()
{
	DrawMoney(m_pPlayer); // 所持金額表示

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_ENHANCE");
#endif
}
