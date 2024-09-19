#include "DxLib.h"
#include "Input.h"
//#include "SceneTitle.h"
//#include "SceneSelectStage.h"
//#include "SceneOption.h"
//#include "SceneStage1.h"
//#include "SceneStage2.h"
//#include "SceneClear.h"
//#include "SceneGameover.h"
#include "SceneDebug.h"

// �萔
namespace
{
	constexpr int kTextColor = 0xffffff;		// �����F
	constexpr int kSelectTextColor = 0xff0000;	// �I�𒆂̕����F
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneDebug::SceneDebug()
{
	m_select = SelectScene::kTitle;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneDebug::Update(Input& input)
{
	// �I����ԍX�V
	UpdateSelect(input, kSelectNum);

	// �J��
	if (input.IsTriggered("OK"))
	{
		//if (m_select == SelectScene::kTitle)
		//{
		//	return std::make_shared<SceneTitle>();
		//}
		//else if (m_select == SelectScene::kStageSelect)
		//{
		//	return std::make_shared<SceneSelectStage>();
		//}
		//else if (m_select == SelectScene::kOption)
		//{
		//	return std::make_shared<SceneOption>(shared_from_this());
		//}
		//else if (m_select == SelectScene::kStage1)
		//{
		//	return std::make_shared<SceneStage1>(pPlayer, pCamera, pStage);
		//}
		//else if (m_select == SelectScene::kStage2)
		//{
		//	return std::make_shared<SceneStage2>(pPlayer, pCamera, pStage);
		//}
		//else if (m_select == SelectScene::kClear)
		//{
		//	return std::make_shared<SceneClear>();
		//}
		//else if (m_select == SelectScene::kGameover)
		//{
		//	return std::make_shared<SceneGameover>(shared_from_this());
		//}
	}

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneDebug::Draw()
{
	int titleColor = kTextColor;
	int selectColor = kTextColor;
	int stageColor = kTextColor;
	int debugColor = kTextColor;
	int optionColor = kTextColor;
	int endingColor = kTextColor;

	if (m_select == SelectScene::kTitle) titleColor = kSelectTextColor;
	if (m_select == SelectScene::kSelect) selectColor = kSelectTextColor;
	if (m_select == SelectScene::kStage) stageColor = kSelectTextColor;
	if (m_select == SelectScene::kDebug) debugColor = kSelectTextColor;
	if (m_select == SelectScene::kOption) optionColor = kSelectTextColor;
	if (m_select == SelectScene::kEnding) endingColor = kSelectTextColor;

	DrawString(0, 20, "�^�C�g��", titleColor);
	DrawString(0, 40, "�Z���N�g", selectColor);
	DrawString(0, 60, "�X�e�[�W", stageColor);
	DrawString(0, 80, "�f�o�b�O�p�X�e�[�W", debugColor);
	DrawString(0, 100, "�I�v�V����", optionColor);
	DrawString(0, 120, "�G���f�B���O", endingColor);
}