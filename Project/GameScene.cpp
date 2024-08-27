#include <DxLib.h>
#include "UseSTL.h"
//#include "VECTORtoUseful.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "UIManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScene::GameScene()
	: gameState(this->TITLE)
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameScene::~GameScene()
{
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
	map.Initialize();
	enemy.Initialize();
	player.Initialize();
	ui.Initialize();
}

/// <summary>
/// �ŏI����
/// </summary>
void GameScene::Finalize()
{
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
	auto& collision   = Singleton<CollisionManager>	 ::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*���W�ړ�*/
	player.Action();
	enemy.Action();

	/*���W�̕␳���s��*/
	collision.HitCheck();

	/*�X�V����*/
	input.Update();
	debug.Update();
	camera.Update();
	map.Update();
	enemy.Update();
	player.Update();
	ui.Update();

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
	auto& collision	  = Singleton<CollisionManager>	 ::GetInstance();
	auto& ui		  = Singleton<UIManager>::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	debug.Draw();
	enemy.Draw();
	player.Draw();
	collision.DebugDrawHit();
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
