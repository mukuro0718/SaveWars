#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "CharacterData.h"
#include "PlayerBlock.h"
#include "UseJson.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerBlock::PlayerBlock()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerBlock::~PlayerBlock()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerBlock::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerBlock::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerBlock::Update(Player& _player)
{
	/*��]�����o��*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	/*�ړ����x���O�ȏゾ�����珈�����s��*/
	if (nowSpeed > maxSpeed)
	{
		//�ړ����x���o��
		UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
		_player.SetSpeed(nowSpeed);
		//�ړ��x�N�g�����o��
		VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
		VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
		_player.SetVelocity(newVelocity);
	}

	/*�K�[�h���������Ă��邩�̔��������*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR enemyFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);		 //�G���ŏ��Ɍ����Ă������
	VECTOR playerFirstDirection = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);	 //�v���C���[���ŏ��Ɍ����Ă������
	VECTOR enemyDirection		= VTransform(enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));	 //���̃G�l�~�[�̌���
		   enemyDirection		= VNorm(enemyDirection);
	VECTOR playerDirection		= VTransform(playerFirstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));//���̃v���C���[�̌���
		   playerDirection		= VNorm(playerDirection);
	float  TOLERANCE_DOT		= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];//���ς̋��e�͈�
	float  enemyAndPlayerDot	= VDot(enemyDirection, playerDirection);
		   enemyAndPlayerDot	= enemyAndPlayerDot * 180.0f / DX_PI_F;
	//�������ς��}�C�i�X�Ȃ�{�ɖ߂��Ĕ��肷��
	if (enemyAndPlayerDot < 0) { enemyAndPlayerDot *= -1.0f; }
	//���ς����e�͈͂��X�^�~�i������Ă�����K�[�h����
	if (enemyAndPlayerDot >= TOLERANCE_DOT && _player.CanAction(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]))
	{
		_player.GetPlayerData().isGuard = true;
	}
	else
	{
		_player.GetPlayerData().isGuard = false;
	}

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::BLOCK);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}