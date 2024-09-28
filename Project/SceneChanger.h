//===============================================
// @brief �V�[���`�F���W���[�N���X
// �V�[���^�C�v�̕ύX�֐�������
// �V�[���̃^�C�v��ێ�
// �V���O���g���Ŏ���
//===============================================
#pragma once
#include "Singleton.h"

class SceneChanger :public Singleton<SceneChanger>
{
public:
	friend class Singleton<SceneChanger>;

	//�V�[���^�C�v
	enum class SceneType
	{
		TITLE		= 0,
		TUTORIAL	= 1,
		GAME		= 2,//�Q�[��
		GAME_CLEAR	= 3,
		GAME_OVER	= 4,
	};

	void SynchroScene();							//�V�[���̓���
	void ChangeScene (const SceneType _nextScene);	//�V�[���̕ύX
	
	/*getter*/
	const SceneType GetNowSceneType ()const { return this->nowScene; }		//���݂̃V�[���^�C�v��getter
	const SceneType GetNextSceneType()const { return this->nextSceneType; } //���̃V�[���^�C�v��getter
private:
	/*���������֐�*/
			 SceneChanger();//�R���X�g���N�^
	virtual ~SceneChanger();//�f�X�g���N�^

	/*�����o�ϐ�*/
	SceneType			 nowScene;		//���݂̃V�[��
	SceneType			 nextSceneType;	//���̃V�[��
};