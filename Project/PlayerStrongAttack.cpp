#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerStrongAttack.h"
#include "PlayerAttackManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStrongAttack::PlayerStrongAttack()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStrongAttack::~PlayerStrongAttack()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerStrongAttack::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->isPlay = false;
	this->isCharge = true;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerStrongAttack::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerStrongAttack::Update(Player& _player)
{
	/*�A�j���[�V�������I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
		return;
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*���ߔ���*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->isCharge)
	{
		auto& input = Singleton<InputManager> ::GetInstance();
		//�U���{�^���𒷉������Ă�����
		if (input.GetNowPadState() & InputManager::PAD_Y)
		{
			this->frameCount++;
			//���̃t���[���𒴂�����U�����ĊJ����
			if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_MAX_FRAME"])
			{
				this->isCharge = false;
			}
		}
		//�U���{�^���������Ă��Ȃ�������A�U�����ĊJ����
		else
		{
			this->isCharge = false;
		}
	}
	//���߂Ă��Ȃ����
	else
	{
		if (!this->isPlay)
		{
			auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
			//�_���[�W�̌v�Z
			int damageFactor = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE_FACTOR"];
			int baseDamage = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE"];
			int damage = baseDamage * (this->frameCount / damageFactor);
			//�x�[�X�����������Ȃ�Ȃ��悤�ɂ���
			if (damage < baseDamage)
			{
				damage = baseDamage;
			}
			//�_���[�W�̃Z�b�g
			attack.SetDamage(damage);
			//�U���̊J�n
			attack.OnIsStart();
			this->isPlay = true;
		}
	}

	/*�A�j���[�V�����̍X�V*/
	//���ߍU�����ɍU���A�j���[�V������~�t���[���𒴂��Ă�����A�j���[�V�������~����
	bool isStopAnimation = false;
	if (this->isCharge && this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_FRAME"])
	{
		isStopAnimation = true;
	}
	//��~�t���O�������Ă��Ȃ���΍Đ�����
	if (!isStopAnimation)
	{
		int nextAnimation = static_cast<int>(Player::AnimationType::SKILL);
		int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
		_player.PlayAnimation(nextAnimation, playTime);
	}

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isChangeAction = true;
	}
}