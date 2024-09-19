#include "SceneBase.h"
#include "Input.h"

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
