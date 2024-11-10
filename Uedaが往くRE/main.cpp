#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Game.h"
#include "LoadCsv.h"
#include "Input.h"
#include "Sound.h"
#include "EffectManager.h"
#include "SceneManager.h"

/// <summary>
/// DxLibの初期化
/// </summary>
/// <param name="titleName">タイトル名</param>
/// <param name="width">スクリーンの横幅</param>
/// <param name="height">スクリーンの高さ</param>
/// <returns>正常に終了したか</returns>
int InitDxLib(const char* titleName, int width, int height)
{
	SetMainWindowText(titleName);

	// windowモード設定
	ChangeWindowMode(true);
	// 画面サイズ変更
	SetGraphMode(width, height, Game::kColorDepth);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	return 0;
}

/// <summary>
/// ゲーム終了
/// </summary>
/// <returns>正常に終了したか</returns>
int EndGame()
{
	Sound::GetInstance().UnLoad();
	Effkseer_End();
	DxLib_End();
	return 0;
}

/// <summary>
/// メイン
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitDxLib("Uedaが往くRE:", Game::kScreenWidth, Game::kScreenHeight);

	// Effekseerの初期化
	Effekseer_Init(8000);

	// フルスクリーン切り替え時におかしくならないように
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// 入力状態を取得
	Input input;
	// メッセージのロード
	LoadCsv::GetInstance().LoadMessage();
	// サウンドのロード
	Sound::GetInstance().Load();
	// エフェクトのロード
	EffectManager::GetInstance().Load();

	// SceneManagerを生成
	std::shared_ptr<SceneManager> pScene = std::make_shared<SceneManager>();
	pScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		// 更新
		input.Update();
		pScene->Update(input);

		// 描画
		pScene->Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}


#ifdef _DEBUG
		// デバッグ時はESCキーで終了できるようにする
		if (input.IsTriggered("end"))
		{
			EndGame();
		}
#endif
	}

	EndGame();
	return 0;		// ソフトの終了 
}