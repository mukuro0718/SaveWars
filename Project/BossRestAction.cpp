#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRestAction::BossRestAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRestAction::~BossRestAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossRestAction::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isSetMoveDirection	 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->maxFrame				 = GetRand(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

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
	const float  SPEED		= json.GetJson(JsonManager::FileType::ENEMY)["SIDE_WALK_SPEED"];
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();
		  VECTOR velocity	= VGet(0.0f, 0.0f, 0.0f);			 //����
		  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetNowMoveTarget(MOVE_TARGET);

	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positionToTargetVector = VSub(POSITION, MOVE_TARGET);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
	nowRotation.y = static_cast<float>(atan2(static_cast<double>(positionToTargetVector.x), static_cast<double>(positionToTargetVector.z)));

	/*��]������*/
	_boss.SetRotation(nowRotation);


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
	_boss.PlayAnimation();

	//�t���[���v��
	bool isEndCount = FrameCount(this->maxFrame);
	//�t���[���v�����I�����Ă�����
	if (isEndCount)
	{
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"]);
		this->maxFrame = GetRand(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	/*�C���^�[�o�����c���Ă�����~���l��0�ɂ���*/
	if (this->parameter->interval != 0)
	{
		this->parameter->desireValue = 0;
		this->parameter->interval--;
		return;
	}

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	else if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		this->parameter->desireValue = 0;
	}

	else
	{
		this->parameter->desireValue++;
	}
}