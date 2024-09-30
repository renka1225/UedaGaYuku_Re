#pragma once
#include "CharacterBase.h"
#include <string>
#include <map>

/// <summary>
/// シングルトンを使用してCSVファイル読み込みを行う
/// </summary>
class CsvLoad
{
public:
	// コピーコンストラクタの禁止
	CsvLoad(const CsvLoad&) = delete;
	// ムーブコンストラクタの禁止
	CsvLoad(CsvLoad&&) = delete;
	// 代入演算子の禁止
	CsvLoad& operator= (const CsvLoad&) = delete;
	CsvLoad& operator= (CsvLoad&&) = delete;

	// インスタンスを取得する
	static CsvLoad& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new CsvLoad;
		}
		return *m_instance;
	}

	// キャラクターのステータス情報を読み込む
	void LoadStatus(CharacterBase::Status& data, const char* charaName);
	// アニメーション情報を読み込む
	void LoadAnimData(CharacterBase::AnimInfo& data, const char* charaName);

private:
	CsvLoad() = default;
	virtual ~CsvLoad() = default;

private:
	static CsvLoad* m_instance;				   // メッセージのインスタンス
	std::map<std::string, std::string> m_data; // メッセージのデータ
};

