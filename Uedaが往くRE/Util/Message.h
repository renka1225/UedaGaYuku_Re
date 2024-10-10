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

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static Message& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Message;
		}
		return *m_instance;
	}

	/// /// <summary>
	/// csvファイルからメッセージを読み込む
	/// </summary>
	void Load();

	/// <summary>
	/// string型のメッセージの取得
	/// </summary>
	/// <param name="id">取得したいメッセージのID</param>
	/// <returns>メッセージ</returns>
	std::string Get(std::string id);

	/// <summary>
	/// char型のメッセージの取得
	/// </summary>
	/// <param name="id">取得したいメッセージのID</param>
	/// <returns>メッセージ</returns>
	const char* Get_c(std::string id);

private:
	Message() = default;
	virtual ~Message() = default;

private:
	static Message* m_instance;				   // メッセージのインスタンス
	std::map<std::string, std::string> m_data; // メッセージのデータ
};