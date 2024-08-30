#include <DxLib.h>
#include "GoriLib.h"
#include "UseSTL.h"
#include "GameObjectTag.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "UIManager.h"

using namespace GoriLib;

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScene::GameScene()
	: gameState(this->TITLE)
{
	this->physics = new GoriLib::Physics;
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameScene::~GameScene()
{
	delete(this->physics);
}

/// <summary>
/// ������
/// </summary>
void GameScene::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	
	/*������*/
	camera.Initialize();
	map.Initialize(this->physics);
	enemy.Initialize(this->physics);
	player.Initialize(this->physics);
	ui.Initialize();
}

/// <summary>
/// �ŏI����
/// </summary>
void GameScene::Finalize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	map.Finalize(this->physics);
	enemy.Finalize(this->physics);
	player.Finalize(this->physics);
}

/// <summary>
/// �X�V
/// </summary>
void GameScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input		  = Singleton<InputManager>		 ::GetInstance();
	auto& debug		  = Singleton<Debug>			 ::GetInstance();
	auto& camera	  = Singleton<CameraManager>	 ::GetInstance();
	auto& map		  = Singleton<MapManager>		 ::GetInstance();
	auto& player	  = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy		  = Singleton<EnemyManager>		 ::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�X�V����*/
	input.Update();
	debug.Update();
	camera.Update();
	map.Update(this->physics);
	enemy.Update(this->physics);
	player.Update(this->physics);
	ui.Update();
	this->physics->Update();
	/*�I������*/
	ChangeState();
}

/// <summary>
/// �`��
/// </summary>
const void GameScene::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& debug		  = Singleton<Debug>			 ::GetInstance();
	auto& map		  = Singleton<MapManager>		 ::GetInstance();
	auto& player	  = Singleton<PlayerManager>	 ::GetInstance();
	auto& camera	  = Singleton<CameraManager>	 ::GetInstance();
	auto& enemy		  = Singleton<EnemyManager>		 ::GetInstance();
	auto& ui		  = Singleton<UIManager>::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	debug.Draw();
	enemy.Draw();
	player.Draw();
	ui.Draw();
}

/// <summary>
/// �I�����邩
/// </summary>
void GameScene::ChangeState()
{
	/*�^�C�g�����ɉ����{�^���������ꂽ��*/
	this->gameState = this->TUTORIAL;

	/*�`���[�g���A�������ׂďI��������*/
	this->gameState = this->GAME;

	/*�{�X�܂��̓v���C���[��HP���O�ɂȂ�����*/
	this->gameState = this->RESULT;

	/*���U���g�̕\�����I�������*/
	this->gameState = this->TITLE;
}
