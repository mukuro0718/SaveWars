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
	, isPlay(false)
{

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
	this->isPlay = false;
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
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*�J�n���ɍs������*/
	if (!this->isPlay)
	{
		//HP���񕜁i�ő�𒴂��Ȃ��悤�ɂ���j
		int hp = _player.GetPlayerData().hp;
		hp += json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
		if (hp >= json.GetJson(JsonManager::FileType::PLAYER)["HP"])
		{
			hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
		}
		_player.GetPlayerData().hp = hp;
		//�񕜉񐔂̌���
		int healCount = _player.GetHealCount();
		healCount--;
		_player.SetHealCount(healCount);
		//�񕜃G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager> ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
		this->isPlay = true;
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::HEAL);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}