#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDrawSword2.h"
#include "PlayerAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerDrawSword2::PlayerDrawSword2()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerDrawSword2::~PlayerDrawSword2()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerDrawSword2::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerDrawSword2::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerDrawSword2::Update(Player& _player)
{

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�A�j���[�V�����̍Đ�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	int nextAnimation = static_cast<int>(Player::AnimationType::DRAW_SWORD_2);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	if (this->frameCount == 0)
	{
		MV1SetFrameVisible(_player.GetModelHandle(), json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_VISIBLE_FRAME"], true);
		_player.SetIsDrawSword(true);
	}

	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_2_CANCEL_FRAME"])
	{
		this->isChangeAction = true;
	}
	/*�A�j���[�V�������I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}