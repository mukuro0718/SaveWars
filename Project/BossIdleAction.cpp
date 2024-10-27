#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossIdleAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossIdleAction::~BossIdleAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossIdleAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = false;
	this->isSetMoveDirection = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->maxFrameCount			 = 0;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	///////////////////////////////
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0 || (_boss.GetNowPhase() != _boss.GetPrevPhase())) { this->OffIsSelect(this->maxFrameCount); return; }

	/*�I������Ă�����~���l���O�ɂ���*/
	this->parameter->desireValue = 0;

	/*�ړ�����������ݒ肳��Ă��Ȃ����*/
	if (!this->isSetMoveDirection)
	{
		//�ړ������̐ݒ�iRIGHT=1�j
		this->directionType = GetRand(static_cast<int>(DirectionType::RIGHT));
		//�t���O�𗧂Ă�
		this->isSetMoveDirection = true;
	}

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����l�̏���*/
	const float  SPEED					= json.GetJson(JsonManager::FileType::ENEMY)["SIDE_WALK_SPEED"];			//���x
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();										//�ړ��ڕW
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();										//�{�X�̍��W
	const VECTOR LERP_VALUE				= Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);//��]���̕⊮�l
		  VECTOR velocity				= Gori::ORIGIN;																//����
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();										//���݂̉�]��
		  VECTOR nextRotation			= Gori::ORIGIN;																//���̍��W
		  VECTOR positionToTargetVector	= VSub(POSITION,MOVE_TARGET);												//�^�[�Q�b�g���玩���̍��W�܂ł̃x�N�g�����o��

	/*��]����*/
	{
		//�ړ��ڕW�̐ݒ�
		_boss.SetNowMoveTarget(MOVE_TARGET);
		//��]����ۊǂ���
		nowRotation = GetLerpRotation(_boss,positionToTargetVector,nowRotation,LERP_VALUE);
		//��]����ݒ�
		_boss.SetRotation(nowRotation);
	}
	
	/*�X�s�[�h���O�ɂ���*/
	_boss.SetSpeed(SPEED);

	/*��]�������ƂɁA�ړ�����������o��*/
	if (this->directionType == static_cast<int>(DirectionType::RIGHT))
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::WALK_RIGHT));
	}
	else
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		velocity = VScale(velocity, -1.0f);
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::WALK_LEFT));
	}


	/*�����x�N�g���𐳋K��*/
	velocity = VNorm(velocity);

	/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
	VECTOR aimVelocity = VScale(velocity, SPEED);						//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	_boss.PlayAnimation();

	//�t���[���v��
	bool isEndCount = FrameCount(this->maxFrameCount);
	//�t���[���v�����I�����Ă�����
	if (isEndCount)
	{
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"]);
		_boss.SetAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��ėD��t���O������*/
	if (_boss.GetHP() <= 0)
	{
		this->isPriority = false;
		return;
	}


	Boss::AttackType type = _boss.GetPrevAttackType();
	/*�U���R���{���c���Ă��āA�O�ɍU�����Ă��Ȃ���΂�NONE��������*/
	if (_boss.GetAttackComboCount() != 0 && type == Boss::AttackType::NONE)
	{
		this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	}

	/*�{�X��AngryType�����Ƃɍő�t���[�������߂Ă���*/
	int angryState = _boss.GetAngryState();
	this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["IDLE_ACTION_MAX_FRAME"][angryState];
}