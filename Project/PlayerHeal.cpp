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
	this->healValue = json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
	this->maxHp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::HEAL);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
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

	/*�ړ����x���O�ȏ�̎��������s��*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}