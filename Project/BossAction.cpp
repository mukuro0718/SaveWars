#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"	
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "HitStop.h"
#include "Character.h"
#include "BossAttack.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAction::BossAction()
	: parameter		(nullptr)
	, attack		(nullptr)
	, hitStop		(nullptr)
	, isSelect		(false)
	, isInitialize	(false)
	, isPriority	(false)
	, isAllowAction (false)
	, frameCount	(0)
{
	this->parameter = new ActionParameter();
	this->hitStop = new HitStop();
}

/// <summary>
/// �`��
/// </summary>
const void BossAction::Draw()const
{
	if (this->attack != nullptr)
	{
		this->attack->Draw();
	}
}

/// <summary>
/// �~���l�̎擾
/// </summary>
const int BossAction::GetDesireValue() const
{
	return this->parameter->desireValue;
}

/// <summary>
/// �d�݂̎擾
/// </summary>
const int BossAction::GetWeight(const int _sum)const
{
	return this->parameter->GetWeight(_sum);
}

/// <summary>
/// �d�݂̃x�[�X�̎擾
/// </summary>
const int BossAction::GetBaseWeight()const
{
	return this->parameter->BASE_WEIGHT;
}
/// <summary>
/// �w�肵���t���[���̌v��
/// </summary>
bool BossAction::FrameCount(const int _maxFrame)
{
	//�J�E���g�𑝉�������
	this->frameCount++;
	//�����ő���z���Ă�����t���O�������ăJ�E���g������������
	if (this->frameCount >= _maxFrame)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �ϐ��̏�����
/// </summary>
void BossAction::OffIsSelect(const int _maxInterval)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->isPriority = false;
		this->isAllowAction = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
	}
}

/// <summary>
/// �⊮������]���̎擾
/// </summary>
VECTOR BossAction::GetLerpRotation(Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue)
{
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	//���̉�]�������߂�
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(_positionToTargetVector.x), static_cast<double>(_positionToTargetVector.z)));
	//��]����ۊǂ���
	rotation = _boss.Lerp360Angle(_nowRotation, nextRotation, _lerpValue);
	return rotation;
}

/// <summary>
/// �ړ��x�N�g���̌v�Z
/// </summary>
VECTOR BossAction::CalcVelocity(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed)
{
	//��]�������ƂɈړ��x�N�g�����o��
	VECTOR direction = VGet(-sinf(_nowRotation.y), 0.0f, -cosf(_nowRotation.y));
	//�ړ��x�N�g���𐳋K��
	direction = VNorm(direction);
	//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
	VECTOR aimVelocity = VScale(direction, _speed);					 //�Z�o���ꂽ�ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, _prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��

	return newVelocity;
}
