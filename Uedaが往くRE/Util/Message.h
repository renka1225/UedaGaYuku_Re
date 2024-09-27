#pragma once
#include <string>
#include <map>

/// <summary>
/// シングルトンを使用してメッセージデータを管理する
/// </summary>
class Message
{
public:
	// コピーコンストラクタの禁止
	Message(const Message&) = delete;
	// ムーブコンストラクタの禁止
	Message(Message&&) = delete;
	// 代入演算子の禁止
	Message& operator= (const Message&) = delete;
	Message& operator= (Message&&) = delete;

	// インスタンスを取得する
	static Message& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Message;
		}
		return *m_instance;
	}

	// メッセージデータの初回読み込み
	void Load();

	// メッセージの取得
	std::string Get(std::string id);
	const char* Get_c(std::string id);

private:
	Message() = default;
	virtual ~Message() = default;

private:
	static Message* m_instance;				   // メッセージのインスタンス
	std::map<std::string, std::string> m_data; // メッセージのデータ
};