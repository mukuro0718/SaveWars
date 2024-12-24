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
	auto& json = Singleton<JsonManager>  ::GetInstance();
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->maxStamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::BLOCK_REACTION);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

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
	/*�ړ����x���O�ȏ�̎��������s��*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	/*�����̊J�n���Ɉ�x�����s������*/
	if (this->frameCount == 0)
	{
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
		//�X�^�~�i�����炷
		_player.CalcStamina(this->staminaConsumption, this->maxStamina);
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

	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}