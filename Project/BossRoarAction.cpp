#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossRoarAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRoarAction::BossRoarAction()
	: prevAngryState(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRoarAction::~BossRoarAction()
{
}
/// <summary>
/// ������
/// </summary>
void BossRoarAction::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isInitializeColorScale = false;
	this->isFinishedFirstRoar	 = false;
	this->isPriority			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->parameter->interval	 = 0;
	this->prevAngryState		 = 1;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�J���[�X�P�[���̏���*/
	{
		//�ŏ��̙��K�ł͐F��ς��Ȃ�
		if (this->isFinishedFirstRoar)
		{
			//�{�X�̃��f���n���h��
			const int MODEL_HANDLE = _boss.GetModelHandle();
			//�������t���O�������Ă��Ȃ�������J���[�X�P�[���̏�����
			if (!this->isInitializeColorScale)
			{
				//�������t���O�𗧂Ă�
				this->isInitializeColorScale = true;
				//���݂̐F���擾
				for (int i = 0; i < this->baseColorScale.size(); i++)
				{
					this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
				}
			}
			//�J���[�X�P�[���̍X�V(�����ł͐ԐF�ɂȂ�悤�ɂ���)
			for (int i = 0; i < this->nowColorScale.size(); i++)
			{
				const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["ROAR_TARGET_COLOR_SCALE"]);
				const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
				this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
				this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
			}
		}
	}

	/*�{���Ԃ����킹��*/
	this->prevAngryState = _boss.GetAngryState();

	/*�I������Ă�����~���l���O�ɂ���*/
	this->parameter->desireValue = 0;

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::ROAR));

	/*�g�p����l�̏���*/
	const float  SPEED		= 0.0f;								 //�X�s�[�h
	const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();//
		  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);		 //����

	/*�X�s�[�h���O�ɂ���*/
	_boss.SetSpeed(SPEED);
	
	/*��]�������ƂɁA�ړ�����������o��*/
	direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));

	/*�����x�N�g���𐳋K��*/
	direction = VNorm(direction);

	/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
	VECTOR aimVelocity = VScale(direction, SPEED);								//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	
	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();

	/*���K���ɃA�j���[�V�������I�����Ă�����A�I���t���O�������ăt�F�[�Y�𓝈ꂷ��*/
	if (this->isSelect && _boss.GetIsChangeAnimation())
	{
		OffIsSelect(0);
		this->isFinishedFirstRoar = true;
		this->isInitializeColorScale = false;
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��x�����K�����Ă��Ȃ���Ι��K�t���O�ƗD��t���O�𗧂Ă�*/
	if (!this->isFinishedFirstRoar)
	{
		this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
		this->isPriority = true;
	}

	/*AngryStateType��ANGRY�ɂȂ�������K������*/
	int nowAngryState = _boss.GetAngryState();
	if (nowAngryState != this->prevAngryState)
	{
		if (nowAngryState == static_cast<int>(Boss::AngryStateType::ANGRY))
		{
			this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			this->isPriority = true;
		}
		else
		{
			this->prevAngryState = _boss.GetAngryState();
		}
	}
}

