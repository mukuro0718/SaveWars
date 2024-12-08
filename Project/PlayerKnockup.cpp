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
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	_player.GetPlayerData().isInvinvible = false;

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::DOWN_UP);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}