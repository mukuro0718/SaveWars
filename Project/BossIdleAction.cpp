#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossIdleAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
	, NORMAL_MAX_ANGRY_FRAME_COUNT(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NORMAL_IDLE_ACTION_MAX_FRAME"])
	, ANGRY_MAX_ANGRY_FRAME_COUNT(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ANGRY_IDLE_ACTION_MAX_FRAME"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->speed				= 0;
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::IDLE);
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->checkedState		= static_cast<int>(Boss::BossState::NORMAL);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossIdleAction::~BossIdleAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossIdleAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = false;
	this->isInitializeColorScale = false;
	this->isSetMoveDirection	 = false;
	this->isChangeColorScale	 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->prevState				 = 1;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0 ) { OffIsSelect(this->maxFrameCount); return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�J���[�X�P�[���̕ύX*/
	//{
	//	if (!this->isChangeColorScale && 
	//		_boss.GetAngryState() == static_cast<int>(Boss::BossState::NORMAL))
	//	{
	//		this->isChangeColorScale = true;
	//	}

	//	//�������t���O�������Ă����猻�݂̐F���擾����
	//	if (this->isChangeColorScale)
	//	{
	//		//�{�X�̃��f���n���h��
	//		const int MODEL_HANDLE = _boss.GetModelHandle();
	//		//�t���O�������Ă��Ȃ�������J���[�X�P�[���̕ύX
	//		if (!this->isInitializeColorScale)
	//		{
	//			//�J���[�X�P�[���̍X�V(�����ł͐ԐF�ɂȂ�悤�ɂ���)
	//			for (int i = 0; i < this->nowColorScale.size(); i++)
	//			{
	//				this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
	//			}
	//			this->isInitializeColorScale = true;
	//		}
	//		//�J���[�X�P�[���̍X�V(�����ł͐ԐF�ɂȂ�悤�ɂ���)
	//		for (int i = 0; i < this->nowColorScale.size(); i++)
	//		{
	//			const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["IDLE_TARGET_COLOR_SCALE"]);
	//			const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
	//			this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
	//			this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
	//		}
	//	}
	//}

	//�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j
	VECTOR aimVelocity  = VScale(Gori::ORIGIN, this->speed);						//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.SetNowAnimation(this->nextAnimation);
	_boss.SetAnimationPlayTime(this->animationPlayTime);
	_boss.PlayAnimation();

	//�t���[���v��
	bool isEndCount = FrameCount(this->maxFrameCount);
	//�t���[���v�����I�����Ă�����
	if (isEndCount)
	{
		OffIsSelect(this->maxInterval);
		_boss.SetAttackComboCount();
		this->isChangeColorScale = false;
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��ėD��t���O������*/
	if (_boss.GetHP() <= 0)return;

	/*�U���R���{���Ȃ����*/
	if (_boss.GetAttackComboCount() == 0)
	{
		int nowAngryState = _boss.GetAngryState();
		if (nowAngryState == static_cast<int>(Boss::BossState::NORMAL))
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority			 = true;
			this->isChangeColorScale	 = true;
			this->isInitializeColorScale = false;
		}
	}
}