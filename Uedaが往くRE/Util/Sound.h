#pragma once
#include <string>
#include <vector>
#include <map>

class Input;

namespace SoundName
{
	constexpr const char* kBgm_sampleBGM = "sampleBGM.mp3";
	constexpr const char* kBgm_title = "title.mp3";
	constexpr const char* kBgm_battle = "battle.mp3";
	constexpr const char* kBgm_battleEnd = "battleEnd.mp3";
	constexpr const char* kBgm_congestion = "congestion.mp3";
	constexpr const char* kSe_sampleSE = "sampleSE.mp3";
	constexpr const char* kSe_logo = "logo.mp3";
	constexpr const char* kSe_cursor = "cursor.mp3";
	constexpr const char* kSe_select = "select.mp3";
	constexpr const char* kSe_useItem = "useItem.mp3";
	constexpr const char* kSe_walk = "walk.mp3";
	constexpr const char* kSe_run = "run.mp3";
	constexpr const char* kSe_attack = "attack.mp3";
}

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
	/// 読み込んだBGMを1度だけ流す
	/// </summary>
	/// <param name="bgmName">BGM名</param>
	void PlayBgm(std::string bgmName);

	/// <summary>
	/// 読み込んだBGMをループ再生する
	/// </summary>
	/// <param name="bgmName">BGM名</param>
	void PlayLoopBgm(std::string bgmName);

	/// <summary>
	/// 再生中のBGMを止める
	/// </summary>
	/// <param name="bgmName">BGM名</param>
	void StopBgm(std::string bgmName);

	/// <summary>
	/// 再生中のSEを止める
	/// </summary>
	/// <param name="seName">SE名</param>
	void StopSe(std::string seName);

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
	/// BGMの音量を変更する
	/// </summary>
	/// <param name="input">入力状態</param>
	void ChangeBgmVol(Input& input);

	/// <summary>
	/// SEの音量を変更する
	/// </summary>
	/// <param name="input">入力状態</param>
	void ChangeSeVol(Input& input);

	/// <summary>
	/// 音量の割合を0～255から0～100に変換する
	/// </summary>
	/// <param name="vol">現在の音量</param>
	/// <returns>変換後の音量</returns>
	int ChangeVolRate(int vol);

	/// <summary>
	/// 音量の割合を0～100から0～255に変換する
	/// </summary>
	/// <param name="vol"></param>
	/// <returns></returns>
	int RestoreVolRate(int vol);

	/// <summary>
	/// BGMの音量を取得する
	/// </summary>
	/// <returns>BGMの音量</returns>
	int GetBgmVol();

	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>SEの音量</returns>
	int GetSeVol();

	/// <summary>
	/// 特定のBGMの音量を取得する
	/// </summary>
	/// <param name="bgmName">取得したいBGM名</param>
	/// <returns>BGMの音量</returns>
	int GetBgmVol(std::string bgmName);

	/// <summary>
	///  特定のSEの音量を取得する
	/// </summary>
	/// <param name="bgmName">取得したいSE名</param>
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

	std::map<std::string, SoundData> m_bgmData; // サウンドのデータ
	std::map<std::string, SoundData> m_seData; // サウンドのデータ
};

