#include "Game.h"
#include "Input.h"
#include "SaveData.h"
#include "UiBase.h"
#include "Camera.h"
#include "Player.h"
#include "SceneMain.h"
#include "SceneSave.h"

namespace
{
	// 画像の種類
	enum Handle
	{
		kBg,		 // 背景
		kSaveCursor, // カーソル
		kNum		 // ハンドルの数
	};

	// 画像のパス
	const char* kHandlePath[Handle::kNum]
	{
		"data/ui/save/bg.png",
		"data/ui/save/cursor.png",
	};

	const Vec2 kDispSaveCursorPos = { 438.0f, 296.0f };			// セーブ画面のカーソル表示位置
	const Vec2 kDispDateSaveDataPos = { 730.0f, 348.0f };		// 保存された現在時刻の表示位置
	const Vec2 kDispPlayTimeSaveDataPos = { 750.0f, 408.0f };	// 保存されたプレイ時間の表示位置
}

SceneSave::SceneSave(std::shared_ptr<SceneBase> pScene, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pPrevScene = pScene;

	m_handle.resize(Handle::kNum);
	for (int i = 0; i < m_handle.size(); i++)
	{
		m_handle[i] = LoadGraph(kHandlePath[i]);
	}
}

SceneSave::~SceneSave()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<SceneBase> SceneSave::Update(Input& input)
{
	UpdateSaveSelect(input, SaveData::SelectSaveData::kSaveNum);

	// セーブデータを書き込む
	if (input.IsTriggered(InputId::kA))
	{
		SaveData::GetInstance().WriteData(*m_pPlayer, *m_pCamera, m_playTime, m_saveSelect);
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
	DrawGraph(0, 0, m_handle[Handle::kBg], true);
	m_pUi->DrawSaveCursor(kDispSaveCursorPos, m_saveSelect);
	DrawMoney(m_pPlayer); // 所持金額表示

	// セーブデータの情報を表示
	SaveData::GetInstance().DrawDateSaveData(kDispDateSaveDataPos); // 保存時刻
	SaveData::GetInstance().DrawPlayTimeSaveData(kDispPlayTimeSaveDataPos); // プレイ時間
}
