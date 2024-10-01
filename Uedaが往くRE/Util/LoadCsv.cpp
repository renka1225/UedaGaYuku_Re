#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFileName = "data/csv/charaStatus.csv";
	const char* const kCharaAnimDataFileName = "data/csv/animData.csv";

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
/// �L�����N�^�[�̃X�e�[�^�X����ǂݍ���
/// </summary>
/// <param name="data">�X�e�[�^�X���</param>
/// <param name="charaName">�L�����N�^�[�̖��O</param>
void LoadCsv::LoadStatus(CharacterBase::Status& data, const char* charaName)
{
	std::ifstream ifs(kCharaStatusFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// �Q�Ƃ������L�������������Ă����珈������߂�
		// MEMO:strcmp ��������r���� ��1���� = ��2�����̏ꍇ0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// �X�e�[�^�X����������
	data.maxHp = std::stof(strvec[1]);
	data.walkSpeed = std::stof(strvec[2]);
	data.runSpeed = std::stof(strvec[3]);
}

/// <summary>
/// �A�j���[�V��������ǂݍ���
/// </summary>
/// <param name="data">�A�j���[�V�������</param>
/// <param name="charaName">�L�����N�^�[�̖��O</param>
void LoadCsv::LoadAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kCharaAnimDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();
	}

	// �A�j���[�V��������ݒ�
	std::string animName = strvec[0];
	data[animName].number = std::stoi(strvec[1]);
	data[animName].loopFrame = std::stof(strvec[2]);
	data[animName].endFrame = std::stof(strvec[3]);
	data[animName].playSpeed = std::stof(strvec[4]);
}
