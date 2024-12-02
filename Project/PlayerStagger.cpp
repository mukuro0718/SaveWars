#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
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
	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�����̊J�n���Ɉ�x�����s������*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	if (this->frameCount == 0)
	{
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//�X�s�[�h�̐ݒ�
		const CharacterData& data = _player.GetCharacterData();
		_player.SetSpeed(json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][data.playerReaction]);
		//�q�b�g�X�g�b�v�̐ݒ�
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
	}

	/*�t���[���v��*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::REACTION);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}