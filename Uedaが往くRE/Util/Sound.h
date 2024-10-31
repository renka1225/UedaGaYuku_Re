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
	/// <param name="seName">SE名</param>
	void PlaySe(std::string seName);

	/// <summary>
	/// 読み込んだBGMを流す
	/// </summary>
	/// <param name="bgmName">BGM名</param>
	void PlayBgm(std::string bgmName);

	/// <summary>
	/// 再生中のBGMを止める
	/// </summary>
	/// <param name="bgmName">BGM名</param>
	void StopBgm(std::string bgmName);

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
	int GetBgmVol(std::string bgmName);

	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>SEの音量</returns>
	int GetSeVol(std::string seName);

	/// <summary>
	/// 現在SEが流れているか取得する
	/// </summary>
	/// <param name="seName">チェックするSE名</param>
	/// <returns>流れているかどうか</returns>
	bool GetIsPlaySe(std::string seName);

	/// <summary>
	/// 現在BGMが流れているか取得する
	/// </summary>
	/// <param name="bgmName">チェックするBGM名</param>
	/// <returns>流れているかどうか</returns>
	bool GetIsPlayBgm(std::string bgmName);

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

