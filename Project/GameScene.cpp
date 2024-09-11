#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "GameScene.h"
#include "BitFlag.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "PlayerAttackManager.h"
#include "EnemyManager.h"
#include "BossAttackManager.h"
#include "UIManager.h"
#include "EffectManager.h"
#include "SceneState.h"
#include "CollisionManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScene::GameScene()
	: gameState(nullptr)
{
	this->gameState = new BitFlag();
	this->gameState->SetFlag(this->TITLE);
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
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& enemyAttack = Singleton<BossAttackManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	
	/*������*/
	camera.Initialize();
	map.Initialize();
	player.Initialize();
	enemy.Initialize();
	playerAttack.Initialize();
	enemyAttack.Initialize();
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
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemyAttack = Singleton<BossAttackManager>::GetInstance();

	map.Finalize();
	enemy.Finalize();
	player.Finalize();
	playerAttack.Finalize();
	enemyAttack.Finalize();

}

/// <summary>
/// �X�V
/// </summary>
void GameScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>		 ::GetInstance();
	auto& debug = Singleton<Debug>			 ::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>		 ::GetInstance();
	auto& enemyAttack = Singleton<BossAttackManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& sceneState = Singleton<SceneState>::GetInstance();
	auto& collision = Singleton<CollisionManager>::GetInstance();

	/*�X�V����*/
	input.Update();
	debug.Update();
	camera.Update();
	map.Update();
	enemy.Update();
	player.Update();
	playerAttack.Update();
	enemyAttack.Update();
	effect.Update();
	ui.Update();
	collision.Update();
	sceneState.Update();

	/*�I������*/
	ChangeState();
}

/// <summary>
/// �`��
/// </summary>
const void GameScene::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& debug = Singleton<Debug>			 ::GetInstance();
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>		 ::GetInstance();
	auto& enemyAttack = Singleton<BossAttackManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	debug.Draw();
	enemy.Draw();
	enemyAttack.Draw();
	player.Draw();
	playerAttack.Draw();
	effect.Draw();
	ui.Draw();
}

/// <summary>
/// �I�����邩
/// </summary>
void GameScene::ChangeState()
{
	
}
