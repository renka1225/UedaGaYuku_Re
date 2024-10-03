#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneMain.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneMain::SceneMain()
{
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pStage = std::make_shared<Stage>();
}

/// <summary>
/// デストラクタ
/// </summary>
SceneMain::~SceneMain()
{
}

/// <summary>
/// 初期化
/// </summary>
void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pCamera->Init();
}

/// <summary>
/// 更新
/// </summary>
std::shared_ptr<SceneBase> SceneMain::Update(Input& input)
{
	m_pPlayer->Update(input, *m_pCamera, *m_pStage);
	m_pCamera->Update(input, *m_pPlayer, *m_pStage);

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void SceneMain::Draw()
{
	m_pStage->Draw();
	m_pPlayer->Draw();

#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_PLAYING");
#endif
}
