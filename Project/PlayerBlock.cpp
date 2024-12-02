#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "CharacterData.h"
#include "PlayerBlock.h"
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
	this->isChangeAction = false;
	this->isEndAction = false;
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
	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"], false, true);
	Move(_player, data);

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
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	this->isEndAction = true;
}