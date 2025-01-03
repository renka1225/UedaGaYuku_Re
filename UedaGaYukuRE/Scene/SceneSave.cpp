#include "Game.h"
#include "Input.h"
#include "SaveData.h"
#include "Camera.h"
#include "Player.h"
#include "SceneSave.h"

SceneSave::SceneSave(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pPrevScene = pScene;
}

SceneSave::~SceneSave()
{
}

void SceneSave::Init()
{
}

std::shared_ptr<SceneBase> SceneSave::Update(Input& input)
{
	UpdateSaveSelect(input, SaveData::SelectSaveData::kSaveNum);

	// セーブデータを書き込む
	if (input.IsTriggered(InputId::kA))
	{
		SaveData::GetInstance().WriteData(*m_pPlayer, *m_pCamera, m_saveSelect);
	}

	if (input.IsTriggered(InputId::kBack))
	{
		SoundCancelSe();
		return m_pPrevScene; // メニュー画面に戻る
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneSave::Draw()
{
#ifdef _DEBUG	// デバッグ表示
	DrawSceneText("MSG_DEBUG_SAVE");

	int one = Color::kColorW;
	int two = Color::kColorW;
	int three = Color::kColorW;

	if (m_saveSelect == SaveData::SelectSaveData::one) one = Color::kColorR;
	if (m_saveSelect == SaveData::SelectSaveData::two) two = Color::kColorR;
	if (m_saveSelect == SaveData::SelectSaveData::three) three = Color::kColorR;

	DrawString(0, 40, "セーブデータ:1", one);
	DrawString(0, 60, "セーブデータ:2", two);
	DrawString(0, 80, "セーブデータ:3", three);
#endif
}
