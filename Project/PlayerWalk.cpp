#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerWalk::~PlayerWalk()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerWalk::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerWalk::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerWalk::Update(Player& _player)
{
	/*��]�����o��*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ړ����x���o��*/
	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::WALK_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}