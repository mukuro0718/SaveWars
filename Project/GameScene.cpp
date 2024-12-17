#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "SceneBase.h"
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
#include "UIManager.h"
#include "EffectManager.h"
#include "CollisionManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Timer.h"
#include "FPSController.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScene::GameScene()
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
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& effect = Singleton<EffectManager>		::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();
	
	/*������*/
	//camera.Initialize();
	timer.Initialize();
	map.Initialize();
	player.Initialize();
	enemy.Initialize();
	playerAttack.Initialize();
	ui.Initialize();
	debug.Initialize();
	effect.Initialize();
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

	map.Finalize();
	enemy.Finalize();
	player.Finalize();
	playerAttack.Finalize();

}

/// <summary>
/// �X�V
/// </summary>
void GameScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>		::GetInstance();
	auto& debug = Singleton<Debug>				::GetInstance();
	auto& camera = Singleton<CameraManager>		::GetInstance();
	auto& map = Singleton<MapManager>			::GetInstance();
	auto& player = Singleton<PlayerManager>		::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>		::GetInstance();
	auto& effect = Singleton<EffectManager>		::GetInstance();
	auto& ui = Singleton<UIManager>			::GetInstance();
	auto& collision = Singleton<CollisionManager>	::GetInstance();
	auto& sceneChanger = Singleton<SceneChanger>		::GetInstance();
	auto& hitStop = Singleton<HitStopManager>		::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();

	/*�X�V����*/
	timer.Update();
	debug.Update();
	input.Update();
	hitStop.Update();
	camera.Update();
	map.Update();
	enemy.Update();
	player.Update();
	playerAttack.Update();
	effect.Update();
	collision.Update();
	ui.Update();

	/*�V�[���̏I������*/
	if (this->IsEnd())
	{
		if (player.GetHP() < 0)
		{
			sceneChanger.ChangeScene(SceneChanger::SceneType::GAME_OVER);
		}
		else
		{
			sceneChanger.ChangeScene(SceneChanger::SceneType::GAME_CLEAR);
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameScene::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>		 ::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	enemy.Draw();
	player.Draw();
	playerAttack.Draw();
	effect.Draw();
	ui.Draw();
	timer.Draw();
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
bool GameScene::IsEnd()
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