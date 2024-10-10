#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossIdleAction.h"

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
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::IDLE));

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const float  SPEED = 0.0f;
	const VECTOR ROTATION = _boss.GetRigidbody().GetRotation();
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);			 //����

	/*�X�s�[�h���O�ɂ���*/
	_boss.SetSpeed(SPEED);

	/*��]�������ƂɁA�ړ�����������o��*/
	direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));

	/*�����x�N�g���𐳋K��*/
	direction = VNorm(direction);

	/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
	VECTOR aimVelocity = VScale(direction, SPEED);								//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();

	//�t���[���v��
	bool isEndCount = FrameCount(this->maxFrameCount);
	//�t���[���v�����I�����Ă�����
	if (isEndCount)
	{
		OffIsSelect(0);
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

	///*����HP���O�ȉ���������~���l���O�ɂ��ėD��t���O������*/
	//if (_boss.GetHP() <= 0)
	//{
	//	this->isPriority = false;
	//	return;
	//}

	///*�U���R���{���c���Ă��Ȃ�������~���l���ő�ɂ���B*/
	//else if (_boss.GetAttackComboCount() == 0)
	//{
	//	this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	//	this->isPriority = true;
	//}

	///*�{�X��AngryType�����Ƃɍő�t���[�������߂Ă���*/
	//int angryState = _boss.GetAngryState();
	//this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["IDLE_ACTION_MAX_FRAME"][angryState];
}