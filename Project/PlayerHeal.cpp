#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerHeal.h"
#include "UseJson.h"
#include "Debug.h"
#include "EffectManager.h"



/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerHeal::PlayerHeal()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->healValue				= json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
	this->maxHp					= json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::HEAL);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerHeal::~PlayerHeal()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerHeal::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerHeal::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerHeal::Update(Player& _player)
{
	/*�X�^�~�i�̉�*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*�J�n���ɍs������*/
	if (this->frameCount == 0)
	{
		//HP���񕜁i�ő�𒴂��Ȃ��悤�ɂ���j
		int hp = _player.GetPlayerData().hp;
		hp += this->healValue;
		if (hp >= this->maxHp)
		{
			hp = this->maxHp;
		}
		_player.GetPlayerData().hp = hp;
		//�񕜉񐔂̌���
		int healCount = _player.GetHealCount();
		healCount--;
		_player.SetHealCount(healCount);
		//�񕜃G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager> ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
		this->frameCount++;
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

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}