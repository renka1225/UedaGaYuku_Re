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

	// インスタンスを取得する
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	// キャラクターのステータス情報を読み込む
	void LoadStatus(CharacterBase::Status& data, std::string charaName);
		// 当たり判定のデータを読み込む
	void LoadColData(CharacterBase::ColData& data, std::string charaName);
	// アニメーション情報を読み込む
	void LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data);

private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;

private:
	static LoadCsv* m_instance;	 // インスタンス
};

