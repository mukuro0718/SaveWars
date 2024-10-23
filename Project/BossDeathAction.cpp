#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
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
	this->isPriority			 = false;
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
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	_boss.PlayAnimation();

	/*�����A�j���[�V�������I�����Ă�����*/
	if (_boss.GetIsChangeAnimation())
	{
		_boss.OffIsAlive();
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossDeathAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*HP���O�ȉ���������~���l���ő�ɂ��A�D��t���O�𗧂Ă�*/
	if (_boss.GetHP() <= 0)
	{
		this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
		this->isPriority = true;
	}
	/*����ȊO�Ȃ�~���l���O�ɂ��ėD��t���O������*/
	else
	{
		this->parameter->desireValue = 0;
		this->isPriority = false;
	}
}