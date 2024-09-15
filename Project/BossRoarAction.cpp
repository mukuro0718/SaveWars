#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossRoarAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRoarAction::BossRoarAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRoarAction::~BossRoarAction()
{

}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const float  SPEED		= 0.0f;								 //�X�s�[�h
	const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();//
		  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);		 //����

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

	/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());

	/*���K���ɃA�j���[�V�������I�����Ă�����A�I���t���O�������ăt�F�[�Y�𓝈ꂷ��*/
	if (this->isSelect && _boss.GetIsChangeAnimation())
	{
		_boss.UnifyPhases();
		this->OffIsSelect(0);
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::CalcParameter(const Boss& _boss)
{
	/*�ǉ�����~���l*/
	int addDesireValue = 0;

	/*�����t�F�[�Y���قȂ��Ă�����~���l���ő�ɂ���*/
	if (_boss.GetNowPhase() != _boss.GetPrevPhase())
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*HP���O�ȉ���������~���l���O�ɂ���*/
	else if (_boss.GetHP() <= 0)
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}