#include "DxLib.h"
#include "Input.h"
#include "ItemBase.h"
#include "Player.h"
#include "SceneUseItem.h"

SceneUseItem::SceneUseItem(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPrevScene = pScene;
	m_handle = LoadGraph("data/ui/bg_useItem.png");
	m_pItem = std::make_shared<ItemBase>();
}

SceneUseItem::~SceneUseItem()
{
	DeleteGraph(m_handle);
}

void SceneUseItem::Init()
{
}

std::shared_ptr<SceneBase> SceneUseItem::Update(Input& input)
{
	if (input.IsTriggered(InputId::kBack))
	{
		return m_pPrevScene; // メニュー画面に戻る
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneUseItem::Draw()
{
	DrawGraph(0, 0, m_handle, true);
	DrawMoney(m_pPlayer); // 所持金額表示

#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_USEITEM");
	m_pItem->Draw();
#endif
}
