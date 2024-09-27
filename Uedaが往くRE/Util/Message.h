#pragma once
#include <string>
#include <map>

/// <summary>
/// �V���O���g�����g�p���ă��b�Z�[�W�f�[�^���Ǘ�����
/// </summary>
class Message
{
public:
	// �R�s�[�R���X�g���N�^�̋֎~
	Message(const Message&) = delete;
	// ���[�u�R���X�g���N�^�̋֎~
	Message(Message&&) = delete;
	// ������Z�q�̋֎~
	Message& operator= (const Message&) = delete;
	Message& operator= (Message&&) = delete;

	// �C���X�^���X���擾����
	static Message& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Message;
		}
		return *m_instance;
	}

	// ���b�Z�[�W�f�[�^�̏���ǂݍ���
	void Load();

	// ���b�Z�[�W�̎擾
	std::string Get(std::string id);
	const char* Get_c(std::string id);

private:
	Message() = default;
	virtual ~Message() = default;

private:
	static Message* m_instance;				   // ���b�Z�[�W�̃C���X�^���X
	std::map<std::string, std::string> m_data; // ���b�Z�[�W�̃f�[�^
};