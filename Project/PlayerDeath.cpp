#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDeath.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerDeath::PlayerDeath()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerDeath::~PlayerDeath()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerDeath::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerDeath::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerDeath::Update(Player& _player)
{
	if (!_player.GetIsAlive()) return;

	/*�ړ����x���o��*/
	VECTOR nextRotation = _player.GetRigidbody().GetRotation();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	auto& json = Singleton<JsonManager>::GetInstance();
	bool isRun = false;
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::DEATH);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		_player.DeathProcess();
	}
}