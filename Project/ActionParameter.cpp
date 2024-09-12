#include "ActionParameter.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ActionParameter::ActionParameter()
	: desireValue(0)
	, interval	 (0)
{

}


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