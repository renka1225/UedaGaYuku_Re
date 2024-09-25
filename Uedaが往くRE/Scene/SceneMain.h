#pragma once
#include "SceneBase.h"
#include <memory>

class Player;
class Camera;
class Stage;

/// <summary>
/// ���C���V�[��
/// </summary>
class SceneMain :public SceneBase
{
public:
	SceneMain();
	virtual ~SceneMain();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	std::shared_ptr<Player> m_pPlayer;	// �v���C���[�̃|�C���^
	std::shared_ptr<Camera> m_pCamera;	// �J�����̃|�C���^
	std::shared_ptr<Stage> m_pStage;	// �X�e�[�W�̃|�C���^
};

