#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossDeathAction.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossDeathAction::BossDeathAction()
{
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
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
	/*�I������Ă�����*/
	if (this->parameter->desireValue != 0)
	{
		this->parameter->desireValue = 0;//�~���l���O�ɂ���
		_boss.SetNowAnimation(this->nextAnimation);//�A�j���[�V�����̐ݒ�
		_boss.SetAnimationPlayTime(this->animationPlayTime);//�A�j���[�V�����Đ����Ԃ̐ݒ�

	}

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const float  SPEED = 0.0f;								 //�X�s�[�h
		const VECTOR ROTATION = _boss.GetRigidbody().GetRotation();//��]��
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);		 //����
		//�X�s�[�h�̐ݒ�
		_boss.SetSpeed(SPEED);
		//��]�������ƂɁA�ړ�����������o��
		direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		//�����x�N�g���𐳋K��
		direction = VNorm(direction);
		//�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j
		VECTOR aimVelocity = VScale(direction, SPEED);					//�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				//�O�̈ړ��x�N�g��
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
		//�ړ��x�N�g���̐ݒ�
		_boss.SetVelocity(newVelocity);
	}


	/*�A�j���[�V�����̍Đ�*/
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
	/*HP���O�ȉ���������~���l���ő�ɂ��A�D��t���O�𗧂Ă�*/
	if (_boss.GetHP() <= 0)
	{
		this->parameter->desireValue = this->maxDesireValue;
		this->isPriority = true;
	}
	/*����ȊO�Ȃ�~���l���O�ɂ��ėD��t���O������*/
	else
	{
		this->parameter->desireValue = 0;
		this->isPriority = false;
	}
}