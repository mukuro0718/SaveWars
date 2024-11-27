#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRun.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerRun::PlayerRun()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerRun::~PlayerRun()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerRun::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerRun::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerRun::Update(Player& _player)
{
	/*��]�����o��*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ő呬�x���o��*/
	auto& json = Singleton<JsonManager>::GetInstance();
	bool isRun = false;
	float maxSpeed = 0.0f;
	//�X�^�~�i������Ă���
	if (_player.CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
	}
	//����Ă��Ȃ�
	else
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	}

	/*�X�^�~�i�̌v�Z*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);

	/*�ړ����x���o��*/
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);


	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);
	
	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}