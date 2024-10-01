#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "Character.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAction::BossAction()
	: parameter		(nullptr)
	, isSelect		(false)
	, isInitialize	(false)
	, frameCount	(0)
{
	this->parameter = new ActionParameter();
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

void BossAction::OffIsSelect(const int _maxInterval)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
		this->parameter->SetInterval(_maxInterval);
	}
}