#pragma once
#include "SceneBase.h"

/// <summary>
/// �f�o�b�O�p�V�[��
/// </summary>
class SceneDebug : public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {};
	virtual void Init() {};
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	// �V�[���̎��
	enum SelectScene
	{
		kTitle,			// �^�C�g��
		kSelect,		// �Z���N�g���
		kStage,			// �X�e�[�W
		kDebug,			// �f�o�b�O�p�X�e�[�W
		kOption,		// �I�v�V����
		kEnding,		// �G���f�B���O
		kSelectNum		// �V�[���̐�
	};
};

