#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerKnockdown.h"
#include "EffectManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerKnockdown::PlayerKnockdown()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerKnockdown::~PlayerKnockdown()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerKnockdown::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerKnockdown::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerKnockdown::Update(Player& _player)
{
	/*�X�^�~�i�̉�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		if (this->nextAnimation == static_cast<int>(Player::AnimationType::DOWN_REACTION))
		{
			//�X�e�B�b�N����
			auto& input = Singleton<InputManager> ::GetInstance();
			VECTOR lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
			if (lStick.x != 0.0f || lStick.z != 0.0f)
			{
				this->nextAnimation = static_cast<int>(Player::AnimationType::DOWN_UP);
				_player.GetPlayerData().isInvinvible = false;
			}
		}
		else
		{
			this->isEndAction = true;
		}
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�����̊J�n���Ɉ�x�����s������*/
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
		//���G�t���O�𗧂Ă�
		_player.GetPlayerData().isInvinvible = true;
		this->nextAnimation = static_cast<int>(Player::AnimationType::DOWN_REACTION);
	}

	/*�t���[���v��*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*�A�j���[�V�����̍Đ�*/
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}