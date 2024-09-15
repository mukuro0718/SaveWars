#include "ActionParameter.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ActionParameter::ActionParameter()
	: desireValue(-1)
	, interval	 (0)
{

}

/// <summary>
/// �~���l�̒ǉ�
/// </summary>
void ActionParameter::AddDesireValue(const int _addValue)
{
	this->desireValue += _addValue;

	/*�ő�A�ŏ��͈̔͂���O��Ȃ��悤�ɒ���*/
	if (this->desireValue >= this->MAX_PARAMETER)
	{
		this->desireValue = this->MAX_PARAMETER;
	}
	else if (this->desireValue < 0)
	{
		this->desireValue = 0;
	}
}

/// <summary>
/// �C���^�[�o���̌v�Z
/// </summary>
bool ActionParameter::CalcInterval()
{
	/*�C���^�[�o�����O��葽��������false��Ԃ��ăC���^�[�o�������炷*/
	if (this->interval > 0)
	{
		this->interval--;
		return false;
	}
	return true;
}

/// <summary>
/// �C���^�[�o���̃Z�b�g
/// </summary>
void ActionParameter::SetInterval(const int _set)
{
	/*�����A�N�V�������I������Ă�����*/
	this->interval = _set;
}

const int ActionParameter::GetWeight(const int _sum)const
{
	if (this->desireValue == 0)
	{
		return 0;
	}
	return static_cast<int>(this->desireValue / _sum * this->BASE_WEIGHT);
}