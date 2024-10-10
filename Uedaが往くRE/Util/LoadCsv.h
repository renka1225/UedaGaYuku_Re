#pragma once
#include "CharacterBase.h"
#include <string>
#include <map>

/// <summary>
/// シングルトンを使用してCSVファイル読み込みを行う
/// </summary>
class LoadCsv
{
public:
	// コピーコンストラクタの禁止
	LoadCsv(const LoadCsv&) = delete;
	// ムーブコンストラクタの禁止
	LoadCsv(LoadCsv&&) = delete;
	// 代入演算子の禁止
	LoadCsv& operator= (const LoadCsv&) = delete;
	LoadCsv& operator= (LoadCsv&&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	/// <summary>
	/// キャラクターのステータス情報を読み込む
	/// </summary>
	/// <param name="data">ステータス情報</param>
	/// <param name="charaName">キャラクター名</param>
	void LoadStatus(CharacterBase::Status& data, std::string charaName);

	/// <summary>
	/// 当たり判定のデータを読み込む
	/// </summary>
	/// <param name="data">当たり判定情報</param>
	/// <param name="charType">キャラクター名</param>
	void LoadColData(CharacterBase::ColData& data, std::string charaName);

	/// <summary>
	/// アニメーション情報を読み込む
	/// </summary>
	/// <param name="data">アニメーション情報</param>
	/// <param name="charaName">キャラクター名</param>
	void LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data);

private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;

private:
	static LoadCsv* m_instance;	 // インスタンス
};

