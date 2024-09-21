#include "SceneMain.h"
#include "Player.h"
#include "Camera.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneMain::SceneMain():
	m_pPlayer(nullptr),
	m_pCamera(nullptr)
{
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
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
	m_pPlayer->Update();
	m_pCamera->Update(input, *m_pPlayer);

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void SceneMain::Draw()
{
	m_pPlayer->Draw();

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
#endif
}
