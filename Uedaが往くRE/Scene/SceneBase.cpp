#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "LoadCsv.h"
#include "Vec2.h"
#include "SceneBase.h"

// 定数
namespace
{
	const Vec2 kCurrentScenePos = { 0.0f, 0.0f }; // 現在のシーン名表示位置
}

SceneBase::SceneBase():
	m_select(0)
{
}

SceneBase::~SceneBase()
{
}

#ifdef _DEBUG
void SceneBase::DrawSceneText(const char* sceneName)
{
	DrawStringF(kCurrentScenePos.x, kCurrentScenePos.y, LoadCsv::GetInstance().Get_cMessage(sceneName), Color::kColorW);
}
#endif

void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered(InputId::kDown))
	{
		m_select = (m_select + 1) % selectNum;
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered(InputId::kUp))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
	}
}
