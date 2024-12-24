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
	/*�ړ����x���O�ȏ�̎��������s��*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

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