#pragma once
#include "DxLib.h"
#include "Player.h"
#include <vector>

class Camera;
class Player;

/// <summary>
/// セーブデータの管理を行う
/// </summary>
class SaveData
{
private:
	// セーブデータの情報
	struct SaveDataCore
	{
		// プレイヤー情報
		VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f);	// プレイヤーの現在位置
		float hp = 0.0f;							// 現在のHP量
		float gauge = 0.0f;							// 現在のゲージ量
		int money = 0;								// 現在の所持金
		Player::EnhanceStep enhanceStep;			// 現在の強化段階
		std::vector<int> possessItem;				// 所持アイテム
		int deadEnemyNum = 0;						// 倒した敵数

		// カメラ情報
		VECTOR cameraPos = VGet(0.0f, 0.0f, 0.0f);	// カメラ位置
		VECTOR target = VGet(0.0f, 0.0f, 0.0f);		// カメラの注視点
		float angleH = 0.0f;						// 水平角度
		float angleV = 0.0f;						// 垂直角度

		// 時間情報
		DATEDATA date;		// 現在時刻
		int mplayTime = 0;	// プレイ時間
	};

public:
	// コピーコンストラクタの禁止
	SaveData(const SaveData&) = delete;
	// ムーブコンストラクタの禁止
	SaveData(SaveData&&) = delete;
	// 代入演算子の禁止
	SaveData& operator= (const SaveData&) = delete;
	SaveData& operator= (SaveData&&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static SaveData& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new SaveData;
		}
		return *m_instance;
	}

	// セーブデータ選択
	enum SelectSaveData
	{
		one,
		two,
		three,
		kSaveNum // セーブデータ数
	};

	/// <summary>
	/// セーブデータの読み込み
	/// </summary>
	/// <param name="slot">選択中のスロット番号</param>
	void Load(int slot);

	/// <summary>
	/// セーブデータの書き込み
	/// </summary>
	/// <param name="slot">選択中のスロット番号</param>
	void Write(int slot);

	/// <summary>
	/// セーブデータを削除する
	/// </summary>
	/// <param name="slot">選択中のスロット番号</param>
	void DeleteData(int slot);

	/// <summary>
	/// セーブデータの新規保存
	/// </summary>
	/// <param name="slot">選択中のスロット番号</param>
	void CreateNewData(int slot);

	/// <summary>
	/// セーブデータを書き込む
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	/// <param name="pCamera">カメラ参照</param>
	/// <param name="slot">選択中のスロット番号</param>
	void WriteData(const Player& pPlayer, const Camera& pCamera, int slot);

	/// <summary>
	/// 現在時刻を保存する
	/// </summary>
	void SaveDateData();

	/// <summary>
	/// セーブデータの情報を描画する
	/// </summary>
	/// <param name="slot">選択中のスロット番号</param>
	void DrawSaveData(int slot);

	/// <summary>
	/// セーブデータの情報を取得
	/// </summary>
	/// <returns>セーブデータ</returns>
	SaveDataCore GetSaveData() const { return m_saveData; }

private:
	/// <summary>
	/// セーブデータのパス名を取得する
	/// </summary>
	/// <param name="slot">スロット番号</param>
	/// <returns>パス名</returns>
	std::string GetSaveDataPath(int slot);

private:
	SaveData() = default;
	virtual ~SaveData() = default;
	static SaveData* m_instance; // インスタンス
	SaveDataCore m_saveData{};	 // セーブデータ
};

