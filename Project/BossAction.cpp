#include <DxLib.h>
#include "ActionParameter.h"
#include "BossAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAction::BossAction()
	: parameter(nullptr)
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