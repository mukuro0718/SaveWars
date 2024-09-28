#include <DxLib.h>
#include "SceneManager.h"
#include "SceneBase.h"
#include "GameScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
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
	this->mainScene = new TitleScene   ();//���C���V�[��
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
		/*��ʂ����ꂢ�ɂ���*/
		ClearDrawScreen();
		clsDx();

		/*�V�[���̍X�V*/
		this->mainScene->Update();

		/*�V�[���̕`��*/
		this->fps->Draw();
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


void SceneManager::SceneChange()
{
	auto& changer = SceneChanger::GetInstance();

	if (changer.GetNowSceneType() != changer.GetNextSceneType())
	{
		switch (changer.GetNextSceneType())
		{
		case SceneChanger::SceneType::TITLE:
			delete(this->mainScene);
			this->mainScene = new TitleScene();
			break;
		case SceneChanger::SceneType::TUTORIAL:
			delete(this->mainScene);
			this->mainScene = new TutorialScene();
			break;
		case SceneChanger::SceneType::GAME:
			delete(this->mainScene);
			this->mainScene = new GameScene();
			break;
		case SceneChanger::SceneType::GAME_CLEAR:
			delete(this->mainScene);
			this->mainScene = new GameClearScene();
			break;
		case SceneChanger::SceneType::GAME_OVER:
			delete(this->mainScene);
			this->mainScene = new GameOverScene();
			break;
		}
		changer.SynchroScene();
	}
}