#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "TutorialScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialScene::TutorialScene()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialScene::~TutorialScene()
{
}

/// <summary>
/// ������
/// </summary>
void TutorialScene::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	/*������*/
	player.Initialize();
	enemy.Initialize();
	//camera.Initialize();
	map.Initialize();
	ui.Initialize();

	this->isPrevPressAnyButton = false;
}

/// <summary>
/// �ŏI����
/// </summary>
void TutorialScene::Finalize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// �X�V
/// </summary>
void TutorialScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& camera		= Singleton<CameraManager>	::GetInstance();
	auto& map			= Singleton<MapManager>		::GetInstance();
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger  = Singleton<SceneChanger>	::GetInstance();
	auto& input			= Singleton<InputManager>::GetInstance();

	/*�X�V����*/
	input.Update();
	camera.Update();
	map.Update();
	ui.Update();

	/*�V�[���̏I������*/
	if (this->IsEnd())
	{
		sceneChanger.ChangeScene(SceneChanger::SceneType::GAME);
	}
}

/// <summary>
/// �`��
/// </summary>
const void TutorialScene::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map	 = Singleton<MapManager>	::GetInstance();
	auto& camera = Singleton<CameraManager>	::GetInstance();
	auto& ui	 = Singleton<UIManager>		::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	ui.Draw();
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
bool TutorialScene::IsEnd()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�^�C�g���V�[�����I���\��������true��Ԃ�*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}