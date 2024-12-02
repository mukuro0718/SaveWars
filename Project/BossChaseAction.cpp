#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossChaseAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossChaseAction::BossChaseAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossChaseAction::~BossChaseAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossChaseAction::Initialize()
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
void BossChaseAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::WALK));

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();					//���W
	const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//��]���̕⊮�l
	this->moveTarget = player.GetRigidbody().GetPosition();					//�ړ��ڕW
	VECTOR nowRotation = _boss.GetRigidbody().GetRotation();					//��]��
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);								//����
	const float  SPEED = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];	//����

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetNowMoveTarget(this->moveTarget);

	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positionToTargetVector = VSub(POSITION, this->moveTarget);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
					//��]����⊮����
	nowRotation = GetLerpRotation(_boss, positionToTargetVector, nowRotation, LERP_VALUE);
	//nowRotation.y = static_cast<float>(atan2(static_cast<double>(positionToTargetVector.x), static_cast<double>(positionToTargetVector.z)));

	/*��]������*/
	_boss.SetRotation(nowRotation);

	/*�����̐ݒ�*/
	_boss.SetSpeed(SPEED);

	/*��]�������ƂɁA�ړ�����������o��*/
	direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));

	/*�����x�N�g���𐳋K��*/
	direction = VNorm(direction);

	/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
	VECTOR aimVelocity = VScale(direction, SPEED);								//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	_boss.PlayAnimation();

	/*�ړ��ڕW�Ƃ̋������萔�ȉ���������I���t���O������*/
	float distance = VSize(positionToTargetVector);
	if (distance <= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	{
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["CHASE_INTERVAL"]);
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossChaseAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���������߂�*/
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();	//�ړ��ڕW
	const VECTOR POSITION_TO_TARGET		= VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
	const float  DISTANCE				= VSize(POSITION_TO_TARGET);			//����

	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȏ㗣��Ă�����~���l��{��������*/
	else if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	{
		if (_boss.GetAttackComboCount() == 0)
		{
			this->parameter->desireValue = 0;
		}
		else
		{
			this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
		}
	}
}