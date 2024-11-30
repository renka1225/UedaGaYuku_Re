#include "DxLib.h"
#include "SceneLoad.h"

SceneLoad::SceneLoad():
	m_sSyncLoadNum(0)
{
}

SceneLoad::~SceneLoad()
{
}

void SceneLoad::Init()
{
	//非同期処理を開始する
	SetUseASyncLoadFlag(true);

	// TODO:ここで読み込みを行う

	//非同期処理を終了する
	SetUseASyncLoadFlag(false);
}

void SceneLoad::Update()
{
	// 非同期読み込み中の数を取得する
	m_sSyncLoadNum = GetASyncLoadNum();
}

void SceneLoad::Draw()
{
#ifdef _DEBUG
	DrawSceneText("MSG_DEBUG_LOADING");
#endif // _DEBUG
}
