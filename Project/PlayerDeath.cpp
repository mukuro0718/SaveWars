#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDeath.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerDeath::PlayerDeath()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation = static_cast<int>(Player::AnimationType::DEATH);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerDeath::~PlayerDeath()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerDeath::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = true;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerDeath::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerDeath::Update(Player& _player)
{
	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
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
		_player.DeathProcess();
		this->isEndAction = true;
	}
}