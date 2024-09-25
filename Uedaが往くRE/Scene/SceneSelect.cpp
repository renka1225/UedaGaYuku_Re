#include "DxLib.h"
#include "Game.h"
#include "Input.h"
#include "Message.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "SceneSelect.h"

namespace
{
	// 文字色
	constexpr int kTextColorW = 0xffffff; // 白
	constexpr int kTextColorR = 0xff0000; // 赤
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneSelect::SceneSelect()
{
	m_select = SceneSelect::kMain;
}

/// <summary>
/// デストラクタ
/// </summary>
SceneSelect::~SceneSelect()
{
}

/// <summary>
/// 初期化
/// </summary>
void SceneSelect::Init()
{
}

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// 選択状態更新
	UpdateSelect(input, SelectScene::kSelectNum);

	// 遷移
	if (input.IsTriggered("OK"))
	{
		if (m_select == SelectScene::kMain)
		{
			return std::make_shared<SceneMain>();
		}
		else if (m_select == SelectScene::kOption)
		{
			return std::make_shared<SceneOption>();
		}
	}
	else if (input.IsTriggered("back"))
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneSelect::Draw()
{
#ifdef _DEBUG
	int mainColor = TextColor::kColorW;
	int optionColor = TextColor::kColorW;

	if (m_select == SelectScene::kMain) mainColor = TextColor::kColorR;
	if (m_select == SelectScene::kOption) optionColor = TextColor::kColorR;

	DrawString(0, 60, Message::GetInstance().Get_c("MSG_DEBUG_PLAYING"), mainColor);
	DrawString(0, 80, Message::GetInstance().Get_c("MSG_DEBUG_OPTION"), optionColor);

	DrawSceneText("MSG_DEBUG_SELECT"); // シーン名表示
#endif
}

