#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossDeathAction.h"
#include "EffectManager.h"

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
/// ������
/// </summary>
void BossDeathAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}


/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossDeathAction::Update(Boss& _boss)
{
	/*�I������Ă�����~���l���O�ɂ���*/
	this->parameter->desireValue = 0;

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::DYING));

	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();

	/*�����A�j���[�V�������I�����Ă�����*/
	if (_boss.GetIsChangeAnimation())
	{
		_boss.OffIsAlive();
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_ENTRY);
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