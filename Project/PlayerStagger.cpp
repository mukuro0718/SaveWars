#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ReactionType.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerStagger.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStagger::PlayerStagger()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	this->maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL)];
	this->cancelableFrame = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::REACTION);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStagger::~PlayerStagger()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerStagger::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerStagger::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerStagger::Update(Player& _player)
{

	/*�����̊J�n���Ɉ�x�����s������*/
	if (this->frameCount == 0)
	{
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//�X�s�[�h�̐ݒ�
		const CharacterData& data = _player.GetCharacterData();
		//�q�b�g�X�g�b�v�̐ݒ�
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
	}
	/*�ړ����x���O�ȏ�̎��������s��*/
	if (_player.GetSpeed() != 0)
	{
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
	}

	/*�t���[���v��*/
	if (!this->isChangeAction)
	{
		this->frameCount++;
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
		}
	}

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}