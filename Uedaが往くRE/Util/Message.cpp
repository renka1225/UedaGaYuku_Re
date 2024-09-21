#include "Message.h"
#include <fstream>
#include <sstream>
#include <vector>

Message* Message::m_instance = nullptr;

namespace
{
	const char* const kMessageFileName = "data/csv/message.csv";

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

/// <summary>
/// csvファイルからメッセージを読み込む
/// </summary>
void Message::Load()
{
	std::ifstream ifs(kMessageFileName);
	std::string line;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		std::vector<std::string> strvec = split(line, ',');
		m_data[strvec.at(0)] = strvec.at(1);
	}
}

/// <summary>
/// string型のメッセージの取得
/// </summary>
/// <param name="id">取得したいメッセージのID</param>
/// <returns>メッセージ</returns>
std::string Message::Get(std::string id)
{
	return m_data[id];
}

/// <summary>
/// char型のメッセージの取得
/// </summary>
/// <param name="id">取得したいメッセージのID</param>
/// <returns>メッセージ</returns>
const char* Message::Get_c(std::string id)
{
	return m_data[id].c_str();
}
