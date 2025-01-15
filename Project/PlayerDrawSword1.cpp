#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDrawSword1.h"
#include "PlayerAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerDrawSword1::PlayerDrawSword1()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation = static_cast<int>(Player::AnimationType::DRAW_SWORD_1);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->cancelableFrame = json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_1_CANCEL_FRAME"];
	this->frameIndex = json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_VISIBLE_FRAME"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerDrawSword1::~PlayerDrawSword1()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerDrawSword1::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerDrawSword1::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerDrawSword1::Update(Player& _player)
{
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

	/*�L�����Z���t���O�������Ă��Ȃ�����*/
	if (!this->isChangeAction)
	{
		this->frameCount++;
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
			MV1SetFrameVisible(_player.GetModelHandle(), this->frameIndex, false);
			_player.SetIsDrawSword(false);
		}
	}

	/*�A�j���[�V�������I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}