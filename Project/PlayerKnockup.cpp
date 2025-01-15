#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerKnockup.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerKnockup::PlayerKnockup()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::DOWN_UP);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->canselFrame			= json.GetJson(JsonManager::FileType::PLAYER)["KNOCK_UP_CANCEL_FRAME"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerKnockup::~PlayerKnockup()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerKnockup::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerKnockup::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerKnockup::Update(Player& _player)
{
	/*�X�^�~�i�̉�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*���G�t���O������*/
	if (this->frameCount == 0)
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*��]�̍X�V*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	VECTOR nextRotation = _player.GetNextRotation();
	UpdateRotation(true, nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ړ����x�̍X�V*/
	_player.SetSpeed(0.0f);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	this->frameCount++;
	if (this->canselFrame == this->frameCount)
	{
		this->isChangeAction = true;
	}

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}