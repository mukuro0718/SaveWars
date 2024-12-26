#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo2.h"
#include "EffectManager.h"
#include "HitStop.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerCombo2::PlayerCombo2()
	: PlayerAction()
{
	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, new AttackData());

	/*������*/
	auto& json						  = Singleton<JsonManager>::GetInstance();
	this->hitStopTime				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_TIME"];
	this->hitStopType				  = static_cast<int>(HitStop::Type::SLOW);
	this->hitStopDelay				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_DELAY"];
	this->slowFactor				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_FACTOR"];
	this->collider->radius			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_ATTACK_RADIUS"];					//���a
	this->collider->data->hitStopTime = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_TIME"];					//�q�b�g�X�g�b�v����
	this->collider->data->damage	  = json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][1];						//�_���[�W
	this->nextAnimation				  = static_cast<int>(Player::AnimationType::COMBO_2);										//���̃A�j���[�V����
	this->playTime					  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];//�A�j���[�V�����Đ�����
	this->firstPlayTime				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_ANIM_PLAY_TIME"];				//�A�j���[�V�����̏����Đ�����
	this->cancelableFrame			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_CANCEL_FRAME"];						//�L�����Z���\�t���[��
	this->startHitCheckFrame		  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_HIT_CHECK_FRAME"];			//�����蔻��J�n�t���[��
	this->endHitCheckFrame			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_END_HIT_CHECK_FRAME"];				//�����蔻��I���t���[��
	this->positionOffset			  = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"];							//���W�I�t�Z�b�g�l�t���[��
	this->yOffset					  = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"];							//Y�I�t�Z�b�g�l�t���[��
	this->firstDirection			  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);			//�ŏ��̌���
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
	this->isEndAction	 = false;
	this->frameCount	 = 0;
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));

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
	/*�ړ����x���O�ȏ�̎��������s��*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->playTime;
	if (this->frameCount == 0) playTime = this->firstPlayTime;
	_player.PlayAnimation(this->nextAnimation, playTime);

	/*�L�����Z���t���O�������Ă��Ȃ�����*/
	this->frameCount++;
	if (!this->isChangeAction)
	{
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
		}
	}

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}

	/*�����蔻�肪�J�n���Ă��Ȃ���Α������^�[��*/
	if (this->frameCount < this->startHitCheckFrame) return;

	/*�����蔻�肪�J�n�����^�C�~���O��1�x�����Ă΂��*/
	if (this->frameCount == this->startHitCheckFrame)
	{
		this->collider->data->isDoHitCheck = true;
	}


	/*�U�����������Ă�����G�t�F�N�g���Đ�*/
	if (this->collider->data->isHitAttack)
	{
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_IMPACT);
		effect.SetPosition(EffectManager::EffectType::PLAYER_IMPACT, this->collider->rigidbody.GetPosition());
		this->collider->data->isHitAttack = false;
		_player.SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
	}

	/*�����蔻�苖�t���O�������Ă��Ȃ������瑁�����^�[��*/
	if (!this->collider->data->isDoHitCheck) return;

	/*�����蔻����W�̍X�V*/
	VECTOR direction = VTransform(this->firstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));	//�����̐ݒ�
	VECTOR position	 = _player.GetRigidbody().GetPosition();																//�v���C���[�̍��W
	position		 = VAdd(position, VScale(direction, this->positionOffset));								//�v���C���[�̍��W�ɁA�I�t�Z�b�g�l�𑫂�
	position.y		 += this->yOffset;																						//Y���W�I�t�Z�b�g�l�𑫂�
	this->collider->rigidbody.SetPosition(position);

	//�t���[�����萔�𒴂��Ă���A�����蔻��t���O���~��Ă����瓖���蔻��J�n�t���O������
	if (this->frameCount > this->endHitCheckFrame)
	{
		this->collider->data->isDoHitCheck = false;
	}

#ifdef _DEBUG
	DrawSphere3D(this->collider->rigidbody.GetPosition(), this->collider->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
#endif // _DEBUG

}