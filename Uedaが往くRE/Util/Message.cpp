#include "Message.h"
#include <fstream>
#include <sstream>
#include <vector>

Message* Message::m_instance = nullptr;

namespace
{
	const char* const kMessageFileName = "data/csv/message.csv";

	/// <summary>
	/// ������𕪊�����
	/// </summary>
	/// <param name="input">������</param>
	/// <param name="delimiter">��؂镶��(,)</param>
	/// <returns>��������������</returns>
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
/// csv�t�@�C�����烁�b�Z�[�W��ǂݍ���
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
/// string�^�̃��b�Z�[�W�̎擾
/// </summary>
/// <param name="id">�擾���������b�Z�[�W��ID</param>
/// <returns>���b�Z�[�W</returns>
std::string Message::Get(std::string id)
{
	return m_data[id];
}

/// <summary>
/// char�^�̃��b�Z�[�W�̎擾
/// </summary>
/// <param name="id">�擾���������b�Z�[�W��ID</param>
/// <returns>���b�Z�[�W</returns>
const char* Message::Get_c(std::string id)
{
	return m_data[id].c_str();
}
