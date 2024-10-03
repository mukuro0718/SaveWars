//=======================================
// @brief �V�[���}�l�[�W���[�N���X
// �V�[���̊Ǘ����s��
//=======================================
#pragma once
#include "Singleton.h"

class SceneBase;
class SceneManager : public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>;

	void Update();//�X�V
private:
	/*���������֐�*/
			 SceneManager();//�R���X�g���N�^
	virtual ~SceneManager();//�f�X�g���N�^
	void SceneChange();

	/*�����o�ϐ�*/
	static SceneManager* instance;	//�B��̃C���X�^���X
	SceneBase*			 mainScene;	//���C���V�[���C���X�^���X
	SceneBase*			 loadScene;	//���[�h�V�[���C���X�^���X
};

