#include <DxLib.h>
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "BossRest.h"
#include "PlayerManager.h"

BossRest::BossRest()
{

}

BossRest::~BossRest()
{

}

void BossRest::Initialize()
{
}

void BossRest::Finalize()
{

}

Rigidbody& BossRest::Update(Character& _boss)
{
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRest::CalcParameter(const int _hp, const int _angryValue, const float _distance)
{
	/*�ǉ�����~���l*/
	int addDesireValue = 0;

	/*HP���O�ȉ���������~���l��0�ɂ���*/
	if (_hp <= 0)
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�{��̃Q�[�W�ɔ�Ⴕ�āA�~���Q�[�W�����߂�*/
	else
	{
		this->parameter->desireValue = this->parameter->MAX_PARAMETER - _angryValue;
	}
}