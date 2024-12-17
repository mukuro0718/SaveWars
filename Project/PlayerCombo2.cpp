#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo2.h"
#include "PlayerAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerCombo2::PlayerCombo2()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerCombo2::~PlayerCombo2()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerCombo2::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerCombo2::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerCombo2::Update(Player& _player)
{
	/*�J�n���Ɉ�x�����Ă΂��*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->frameCount == 0)
	{
		auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
		//�_���[�W�̃Z�b�g
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][1]);
		attack.OnIsStart();
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::COMBO_2);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	if (this->frameCount == 0)
	{
		playTime = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_ANIM_PLAY_TIME"];
	}
	_player.PlayAnimation(nextAnimation, playTime);

	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_CANCEL_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}