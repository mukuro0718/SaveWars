#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossIdleAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SIDE_WALK_SPEED"];
	this->rotateLerpValue = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);
	this->nextAnimation = static_cast<int>(Boss::AnimationType::WALK_RIGHT);
	this->nextAnimation2 = static_cast<int>(Boss::AnimationType::WALK_LEFT);
	this->maxInterval = json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->checkedState = static_cast<int>(Boss::BossState::NORMAL);
	this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["IDLE_ACTION_MAX_FRAME"];
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
	this->isInitializeColorScale = false;
	this->isSetMoveDirection	 = false;
	this->isChangeColorScale	 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->maxFrameCount			 = 0;
	this->prevState				 = 1;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0 ) { OffIsSelect(this->maxFrameCount); return; }

	/*�ړ�����������ݒ肳��Ă��Ȃ����*/
	if (!this->isSetMoveDirection)
	{
		this->parameter->desireValue = 0;							//�~���l���O�ɂ���
		this->directionType			 = GetRand(this->RIGHT);	//�ړ������̐ݒ�iRIGHT=1�j
		this->isSetMoveDirection	 = true;						//�t���O�𗧂Ă�
		this->prevState				 = _boss.GetAngryState();		//�{�X�̏�Ԃ��擾
		_boss.SetSpeed(this->speed);							//�X�s�[�h����
		_boss.SetAnimationPlayTime(this->animationPlayTime);//�A�j���[�V�����̍Đ����Ԃ̐ݒ�
		if (this->directionType == this->RIGHT)
		{
			_boss.SetNowAnimation(this->nextAnimation);
		}
		else
		{
			_boss.SetNowAnimation(this->nextAnimation2);
		}
	}

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�J���[�X�P�[���̕ύX*/
	//{
	//	if (!this->isChangeColorScale && 
	//		_boss.GetAngryState() == static_cast<int>(Boss::BossState::NORMAL))
	//	{
	//		this->isChangeColorScale = true;
	//	}

	//	//�������t���O�������Ă����猻�݂̐F���擾����
	//	if (this->isChangeColorScale)
	//	{
	//		//�{�X�̃��f���n���h��
	//		const int MODEL_HANDLE = _boss.GetModelHandle();
	//		//�t���O�������Ă��Ȃ�������J���[�X�P�[���̕ύX
	//		if (!this->isInitializeColorScale)
	//		{
	//			//�J���[�X�P�[���̍X�V(�����ł͐ԐF�ɂȂ�悤�ɂ���)
	//			for (int i = 0; i < this->nowColorScale.size(); i++)
	//			{
	//				this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
	//			}
	//			this->isInitializeColorScale = true;
	//		}
	//		//�J���[�X�P�[���̍X�V(�����ł͐ԐF�ɂȂ�悤�ɂ���)
	//		for (int i = 0; i < this->nowColorScale.size(); i++)
	//		{
	//			const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["IDLE_TARGET_COLOR_SCALE"]);
	//			const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
	//			this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
	//			this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
	//		}
	//	}
	//}

	/*�g�p����l�̏���*/
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();
		  VECTOR velocity				= Gori::ORIGIN;							
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();
		  VECTOR nextRotation			= Gori::ORIGIN;							
		  VECTOR positionToTargetVector	= VSub(POSITION,MOVE_TARGET);		

	/*��]����*/
	{
		//�ړ��ڕW�̐ݒ�
		_boss.SetNowMoveTarget(MOVE_TARGET);
		//��]����ۊǂ���
		nowRotation = GetLerpRotation(_boss, positionToTargetVector, nowRotation, this->rotateLerpValue);
		//��]����ݒ�
		_boss.SetRotation(nowRotation);
	}
	
	/*�ړ�����*/
	//��]�������ƂɁA�ړ�����������o��
	if (this->directionType == this->RIGHT)
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		_boss.SetNowAnimation(this->nextAnimation);
	}
	else
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		velocity = VScale(velocity, -1.0f);
		_boss.SetNowAnimation(this->nextAnimation2);
	}
	//�����x�N�g���𐳋K��
	velocity = VNorm(velocity);
	//�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j
	VECTOR aimVelocity  = VScale(velocity, this->speed);					//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();

	//�t���[���v��
	bool isEndCount = FrameCount(this->maxFrameCount);
	//�t���[���v�����I�����Ă�����
	if (isEndCount)
	{
		OffIsSelect(this->maxInterval);
		_boss.SetAttackComboCount();
		this->isChangeColorScale = false;
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��ėD��t���O������*/
	if (_boss.GetHP() <= 0)return;

	/*�{�X��AngryType���擾*/
	int nowAngryState = _boss.GetAngryState();

	/*AngryState��NORMAL�̎��ɍU���R���{���c���Ă��Ȃ�������*/
	if (nowAngryState == this->checkedState)
	{
		if (nowAngryState != this->prevState)
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority			 = true;
			this->isChangeColorScale	 = true;
			this->isInitializeColorScale = false;
		}
		//�{�X�̍U�����O��������
		else if (_boss.GetAttackComboCount() == 0)
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority = true;
			this->isChangeColorScale = true;
			this->isInitializeColorScale = false;
		}
	}

	/*�Ⴄ��ԂȂ猻�݂̏�Ԃ�ۑ�����*/
	else
	{
		this->prevState = _boss.GetAngryState();
	}
}