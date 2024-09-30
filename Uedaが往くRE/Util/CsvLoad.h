#pragma once
#include "CharacterBase.h"
#include <string>
#include <map>

/// <summary>
/// �V���O���g�����g�p����CSV�t�@�C���ǂݍ��݂��s��
/// </summary>
class CsvLoad
{
public:
	// �R�s�[�R���X�g���N�^�̋֎~
	CsvLoad(const CsvLoad&) = delete;
	// ���[�u�R���X�g���N�^�̋֎~
	CsvLoad(CsvLoad&&) = delete;
	// ������Z�q�̋֎~
	CsvLoad& operator= (const CsvLoad&) = delete;
	CsvLoad& operator= (CsvLoad&&) = delete;

	// �C���X�^���X���擾����
	static CsvLoad& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new CsvLoad;
		}
		return *m_instance;
	}

	// �L�����N�^�[�̃X�e�[�^�X����ǂݍ���
	void LoadStatus(CharacterBase::Status& data, const char* charaName);
	// �A�j���[�V��������ǂݍ���
	void LoadAnimData(CharacterBase::AnimInfo& data, const char* charaName);

private:
	CsvLoad() = default;
	virtual ~CsvLoad() = default;

private:
	static CsvLoad* m_instance;				   // ���b�Z�[�W�̃C���X�^���X
	std::map<std::string, std::string> m_data; // ���b�Z�[�W�̃f�[�^
};

