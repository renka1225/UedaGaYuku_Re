#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Message.h"
#include "Vec2.h"
#include "SceneBase.h"

namespace
{
	const Vec2 kCurrentScenePos = { 0.0f, 0.0f }; // 現在のシーン名表示位置
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneBase::SceneBase():
	m_select(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneBase::~SceneBase()
{
}

#ifdef _DEBUG
/// <summary>
/// 現在のシーンをデバッグ表示する
/// </summary>
void SceneBase::DrawSceneText(const char* sceneName)
{
	DrawStringF(kCurrentScenePos.x, kCurrentScenePos.y, Message::GetInstance().Get_c(sceneName), TextColor::kColorR);
}
#endif

/// <summary>
/// 選択状態更新
/// </summary>
/// <param name="input">入力処理</param>
/// <param name="selectNum">選択肢の数</param>
void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered("down"))
	{
		m_select = (m_select + 1) % selectNum;
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered("up"))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
	}
}
