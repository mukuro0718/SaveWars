#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossDeathAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossDeathAction::BossDeathAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossDeathAction::~BossDeathAction()
{

}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossDeathAction::Update(Boss& _boss)
{
	/*�A�N�V�������I������Ă���Ƃ�*/
	if (this->isSelect)
	{
		//�����A�j���[�V�������I�����Ă�����
		if (_boss.GetIsChangeAnimation())
		{
			_boss.OffIsAlive();
		}
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossDeathAction::CalcParameter(const Boss& _boss)
{
	/*�ǉ�����~���l*/
	int addDesireValue = 0;

	/*HP���O�ȉ���������~���l���ő�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}