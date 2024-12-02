#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerBlockStagger.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerBlockStagger::PlayerBlockStagger()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerBlockStagger::~PlayerBlockStagger()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerBlockStagger::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerBlockStagger::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerBlockStagger::Update(Player& _player)
{
	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
		return;
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�����̊J�n���Ɉ�x�����s������*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	if (this->frameCount == 0)
	{
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
		//�X�^�~�i�����炷
		_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]);
	}


	/*�t���[���v��*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::BLOCK_REACTION);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}