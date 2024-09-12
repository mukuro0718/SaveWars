#include <DxLib.h>
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "BossDeath.h"
#include "PlayerManager.h"

BossDeath::BossDeath()
{

}

BossDeath::~BossDeath()
{

}

void BossDeath::Initialize()
{
}

void BossDeath::Finalize()
{

}

Rigidbody& BossDeath::Update(Character& _boss)
{
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossDeath::CalcParameter(const int _hp, const int _angryValue, const float _distance)
{
	/*�ǉ�����~���l*/
	int addDesireValue = 0;

	/*HP���O�ȉ���������~���l���ő�ɂ���*/
	if (_hp <= 0)
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}