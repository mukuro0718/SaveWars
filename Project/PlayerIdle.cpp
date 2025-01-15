#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerIdle.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerIdle::PlayerIdle()
	: PlayerAction()
	
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::IDLE);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerIdle::~PlayerIdle()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerIdle::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = true;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerIdle::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerIdle::Update(Player& _player)
{
	//int startTime = GetNowCount();
	this->isEndAction = true;

	/*�X�^�~�i�̉�0~1m/s*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

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

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
	//printfDx("PLAYER_FRAMETIME:%d\n", this->frameTime);
}