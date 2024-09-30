#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
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
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();

		/*�g�p����l�̏���*/
		const VECTOR POSITION		= _boss.GetRigidbody().GetPosition();					//���W
		const VECTOR MOVE_TARGET	= player.GetRigidbody().GetPosition();					//�ړ��ڕW
			  VECTOR nowRotation	= _boss.GetRigidbody().GetRotation();					//��]��
			  VECTOR direction		= VGet(0.0f, 0.0f, 0.0f);								//����
		const float  SPEED			= json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];	//����

		/*�ړ��x�N�g���̐ݒ�*/
		_boss.SetNowMoveTarget(MOVE_TARGET);

		/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
		VECTOR positionToTargetVector = VSub(POSITION, MOVE_TARGET);

		/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
		nowRotation.y = static_cast<float>(atan2(static_cast<double>(positionToTargetVector.x), static_cast<double>(positionToTargetVector.z)));

		/*��]������*/
		_boss.SetRotation(nowRotation);

		/*�����̐ݒ�*/
		_boss.SetSpeed(SPEED);

		/*��]�������ƂɁA�ړ�����������o��*/
		direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));

		/*�����x�N�g���𐳋K��*/
		direction = VNorm(direction);

		/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
		VECTOR aimVelocity  = VScale(direction, SPEED);								//�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

		_boss.SetVelocity(newVelocity);

		/*�ړ��ڕW�Ƃ̋������萔�ȉ���������I���t���O������*/
		float distance = VSize(positionToTargetVector);
		if (distance <= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
		{
			OffIsSelect(0);
		}
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossChaseAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ǉ�����~���l*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	///*�����{�X�ƃv���C���[�̊Ԃ��萔�ȏ㗣��Ă�����~���l��{��������*/
	//else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	//{
	//	addDesireValue = this->parameter->MAX_PARAMETER;
	//}
	//else
	//{
	//	addDesireValue = 1;
	//}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}