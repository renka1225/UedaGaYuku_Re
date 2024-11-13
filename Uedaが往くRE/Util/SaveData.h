#pragma once
#include "DxLib.h"

/// <summary>
/// セーブデータの管理を行う
/// </summary>
class SaveData
{
private:
	// セーブデータの情報
	struct SaveDataCore
	{
		VECTOR playerPos;	// プレイヤーの現在位置
		float hp;			// 現在のHP量
		float gauge;		// 現在のゲージ量
		int money;			// 現在の所持金
		// 現在の強化段階
		// 所持アイテム
		int playTime;	// 現在のプレイ時間
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

