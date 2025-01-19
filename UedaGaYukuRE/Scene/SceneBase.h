#pragma once
#include <memory>
#include <vector>
#include <string>

class Input;
class Item;
class UiBase;
class Player;
class Camera;

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <returns>現在のシーンのポインタ</returns>
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// プレイ時間をセットする
	/// </summary>
	/// <param name="playTime">プレイ時間</param>
	void SetPlayTime(int playTime) { m_playTime = playTime; }

protected:
	/// <summary>
	/// ゲーム終了処理
	/// </summary>
	void EndGame();

	/// <summary>
	/// 決定のSEを流す
	/// </summary>
	void SoundSelectSe();

	/// <summary>
	/// キャンセルSEを流す
	/// </summary>
	void SoundCancelSe();

	/// <summary>
	/// シーン遷移の際のサウンド処理
	/// </summary>
	/// <param name="bgmName">再生中のBGM名</param>
	void SceneChangeSound(std::string bgmName);

	/// <summary>
	///	選択状態を更新 
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="selectNum">選択肢の数</param>
	void UpdateSelect(const Input& input, int selectNum);

	/// <summary>
	///	セーブデータの選択状態を更新 
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="selectNum">選択数</param>
	void UpdateSaveSelect(Input& input, int selectNum);

	/// <summary>
	/// 所持金額を表示
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	void DrawMoney(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// フェードイン処理
	/// </summary>
	/// <param name="fadeFrame">フェード変化量</param>
	virtual void FadeIn(int fadeFrame);

	/// <summary>
	/// フェードアウト処理
	/// </summary>
	/// <param name="fadeFrame">フェード変化量</param>
	virtual void FadeOut(int fadeFrame);

	/// <summary>
	/// フェードインアウト描画
	/// </summary>
	void DrawFade();

	/// <summary>
	/// プレイ時間を取得する
	/// </summary>
	/// <returns>現在のプレイ時間</returns>
	int GetPlayTime() const { return m_playTime; }

#ifdef _DEBUG
	/// <summary>
	/// 現在のシーンをデバッグ表示する
	/// </summary>
	/// <param name="sceneName">現在のシーン名</param>
	void DrawSceneText(const char* sceneName);
#endif

protected:
	std::shared_ptr<SceneBase> m_pPrevScene; // 前に実行していたシーン
	std::shared_ptr<Item> m_pItem;			 // アイテムのポインタ
	std::shared_ptr<UiBase> m_pUi;			 // UIのポインタ
	std::shared_ptr<Player> m_pPlayer;		 // プレイヤーのポインタ
	std::shared_ptr<Camera> m_pCamera;		 // カメラのポインタ
	std::vector<int> m_handle;	// ハンドル

	int m_playTime;		// 現在のプレイ時間
	int m_saveSelect;	// セーブデータの選択状態
	int m_select;		// 現在の選択状態
	int m_fadeAlpha;	// フェードのα値
	bool m_isFadeOut;	// フェード中かどうか(true:フェード中)
};

