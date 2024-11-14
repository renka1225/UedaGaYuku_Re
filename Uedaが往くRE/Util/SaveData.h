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

		// カメラ情報
		VECTOR cameraPos = VGet(0.0f, 0.0f, 0.0f);	// カメラ位置
		VECTOR target = VGet(0.0f, 0.0f, 0.0f);		// カメラの注視点
		float angleH = 0.0f;						// 水平角度
		float angleV = 0.0f;						// 垂直角度
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

	/// <summary>
	/// セーブデータの読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// セーブデータの書き込み
	/// </summary>
	void Write();

	/// <summary>
	/// セーブデータを削除する
	/// </summary>
	void DeleteData();

	/// <summary>
	/// セーブデータの新規保存
	/// </summary>
	void CreateNewData();

	/// <summary>
	/// プレイヤーの情報をセットする
	/// </summary>
	/// <param name="pPlayer">プレイヤー参照</param>
	void SetPlayerData(const Player& pPlayer);

	/// <summary>
	/// カメラの情報をセットする
	/// </summary>
	/// <param name="pCamera">カメラ参照</param>
	void SetCameraData(const Camera& pCamera);

	/// <summary>
	/// セーブデータの情報を取得
	/// </summary>
	/// <returns>セーブデータ</returns>
	SaveDataCore GetSaveData() const { return m_saveData; }

private:
	SaveData() = default;
	virtual ~SaveData() = default;
	static SaveData* m_instance; // インスタンス
	SaveDataCore m_saveData;	 // セーブデータ
};

