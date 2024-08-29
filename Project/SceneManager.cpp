#include <DxLib.h>
#include "GoriLib.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "LoadScene.h"
#include "SceneChanger.h"
#include "FPSController.h"

//SceneManager* Singleton<SceneManager>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneManager::SceneManager()
	: mainScene(nullptr)
	, loadScene(nullptr)
	, fps	   (nullptr)
{
	/*�C���X�^���X�̍쐬*/
	this->mainScene = new GameScene	   ();//���C���V�[��
	this->loadScene = new LoadScene	   ();//���[�h�V�[��
	this->fps		= new FPSController();//FPS�R���g���[���[
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{

}
/// <summary>
/// �X�V
/// </summary>
void SceneManager::Update()
{
	if (GetASyncLoadNum() == 0)
	{
		/*�C���v�b�g�}�l�[�W���[�̃C���X�^���X���擾*/
		//auto& input = InputManager::GetInstance();

		/*��ʂ����ꂢ�ɂ���*/
		ClearDrawScreen();
		clsDx();

		/*�C���v�b�g�}�l�[�W���[�̍X�V*/
		//input.Update();

		/*�V�[���̍X�V*/
		this->mainScene->Update();

		/*�V�[���̕`��*/
		this->mainScene->Draw();
		
		/*�V�[���̕ύX*/
		SceneChange();

		/*�e�o�r�̕��ς��Z�o*/
		this->fps->Average();
		
		/*����ʂ̓��e��\�ɔ��f������*/
		ScreenFlip();
		
		/*�e�o�r�̏���*/
		this->fps->Wait();
	}
	else
	{
		this->loadScene->Update();
		this->loadScene->Draw();
	}
}
/// <summary>
/// �V�[���̕ύX
/// </summary>
void SceneManager::SceneChange()
{
	/*�V�[���`�F���W���[�N���X�̃C���X�^���X���擾*/
	auto& changer = SceneChanger::GetInstance();

	/*�������݂̃V�[���^�C�v�ƑO�̃V�[���^�C�v���قȂ��Ă����珈�����s��*/
	if (changer.GetNowSceneType() != changer.GetNextSceneType())
	{
		//�V�[���^�C�v�ɂ���č쐬����C���X�^���X��ς���
		switch (changer.GetNextSceneType())
		{
		case SceneChanger::SceneType::GAME:
			delete(this->mainScene);
			this->mainScene = new GameScene();
			break;
		}
		//�V�[���𓯊�������
		changer.SynchroScene();
	}
}
