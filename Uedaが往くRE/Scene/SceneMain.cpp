#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneMain.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneMain::SceneMain()
{
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneMain::~SceneMain()
{
}

/// <summary>
/// ������
/// </summary>
void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pCamera->Init();
}

/// <summary>
/// �X�V
/// </summary>
std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	m_pPlayer->Update(input, *m_pCamera, *m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneMain::Draw()
{
	m_pStage->Draw();
	m_pPlayer->Draw();

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
#endif
}
