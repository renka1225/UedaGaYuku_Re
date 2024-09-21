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
		kMain,			// �v���C
		kOption,		// �I�v�V����
		kSelectNum		// �V�[���̐�
	};
};

