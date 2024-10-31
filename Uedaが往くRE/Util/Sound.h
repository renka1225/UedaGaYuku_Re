#pragma once
#include <string>
#include <vector>
#include <map>

/// <summary>
/// サウンドの管理を行う
/// </summary>
class Sound
{
public:
	// BGMの種類
	enum BgmKind
	{
		kTitleBGM,	// タイトル
		kSelectBGM,	// 選択画面
		kMainBGM,	// メイン
		kBattleBGM,	// バトル時
		kBgmNum,	// BGMの数
	};

	// SEの種類
	enum SeKind
	{
		kCursorMoveSE,	// カーソル移動
		kSelectSE,		// 決定
		kSeNum			// SEの数
	};

public:
	// コピーコンストラクタの禁止
	Sound(const Sound&) = delete;
	// ムーブコンストラクタの禁止
	Sound(Sound&&) = delete;
	// 代入演算子の禁止
	Sound& operator= (const Sound&) = delete;
	Sound& operator= (Sound&&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static Sound& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Sound;
		}
		return *m_instance;
	}

	/// <summary>
	/// サウンドのデータを読み込む
	/// </summary>
	void Load();

	/// <summary>
	/// 読み込んだデータを削除する
	/// </summary>
	void UnLoad();

	/// <summary>
	/// 読み込んだSEを流す
	/// </summary>
	/// <param name="seKind">SE名</param>
	void PlaySE(SeKind seKind);

	/// <summary>
	/// 読み込んだBGMを流す
	/// </summary>
	/// <param name="bgmKind">BGM名</param>
	void PlayBGM(BgmKind bgmKind);

	/// <summary>
	/// BGMの音量を調整する
	/// </summary>
	/// <param name="bgmName">音量</param>
	void SetBgmVol(std::string bgmName);

	/// <summary>
	/// SEの音量を調整する
	/// </summary>
	/// <param name="seName">音量</param>
	void SetSeVol(std::string seName);

	/// <summary>
	/// BGMの音量を取得する
	/// </summary>
	/// <returns>BGMの音量</returns>
	//int GetBgmVol(std::string BgmName) const { return m_bgmData[BgmName].vol; }

	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>SEの音量</returns>
	//int GetSeVol(std::string seName) const { return m_seData[seName].vol; }

private:
	Sound() = default;
	virtual ~Sound() = default;
	static Sound* m_instance;	 // インスタンス

private:
	struct SoundData
	{
		int handle;
		int vol;
	};
	//std::vector<SoundData> m_bgmData; // BGMのデータ
	//std::vector<SoundData> m_seData;  // SEのデータ

	std::map<std::string, SoundData> m_bgmData; // サウンドのデータ
	std::map<std::string, SoundData> m_seData; // サウンドのデータ
};

