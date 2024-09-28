#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossFlyAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "BossAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossFlyAction::BossFlyAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossFlyAction::~BossFlyAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossFlyAction::Initialize()
{
	this->isSelect = false;
	this->isInitialize = false;
	this->frameCount = 0;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossFlyAction::Update(Boss& _boss)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& effect = Singleton<EffectManager>::GetInstance();
		auto& attack = Singleton<BossAttackManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();


		/*����������Ă��Ȃ�������*/
		if (!this->isInitialize)
		{
			//�G�t�F�N�g�𗧂Ă�
			//effect.();
			//�U���t���O�𗧂Ă�
			attack.OnIsStart(static_cast<int>(BossAttackManager::AttackType::FLY_ATTACK));
			this->isInitialize = true;

			/*��x�����ړ�����*/
			//�g�p����ϐ��̏���
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition(); //���W
			const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();//�ړ��ڕW
			VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //��]��
			VECTOR positonToTargetVector = VSub(POSITION, MOVE_TARGET); //���W�ƈړ��ڕW�Ԃ̃x�N�g��

			//�ړ��x�N�g���̐ݒ�
			_boss.SetNowMoveTarget(MOVE_TARGET);
			//��]����
			{
				//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
				//�A�[�N�^���W�F���g���g���Ċp�x�����߂�
				nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
				//��]������
				_boss.SetRotation(nowRotation);
			}
			//�ړ��X�s�[�h�̐ݒ�
			_boss.SetSpeed(0.0f);
		}

		/*�J�E���g�̌v��*/
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["FLY_ATTACK_SLOW_FRAME_COUNT"]);

		//�ړ��x�N�g�����o��
		{
			VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //��]��
			VECTOR direction = VGet(0.0f, 0.0f, 0.0f);
			float speed = _boss.GetSpeed();

			//�X�s�[�h������
			speed += static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["FLY_ADD_MOVE_SPEED"]);
			//�X�s�[�h�̐ݒ�
			_boss.SetSpeed(speed);
			//��]�������ƂɈړ��x�N�g�����o��
			direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
			//�ړ��x�N�g���𐳋K��
			direction = VNorm(direction);
			//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
			VECTOR aimVelocity = VScale(direction, speed);					 //�Z�o���ꂽ�ړ��x�N�g��
			VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //�O�̈ړ��x�N�g��
			VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
			//�ړ��x�N�g���̐ݒ�
			_boss.SetVelocity(newVelocity);
			
		}

		/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
		{
			float animationPlayTime = _boss.GetAnimationPlayTime();
			//�J�E���g���I�����Ă��Ȃ����
			/*if (isEndCount)
			{
				animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["FLY_SLOW_PLAY_TIME"];
			}*/
			_boss.SetAnimationPlayTime(animationPlayTime);
		}

		/*�I������*/
		if (_boss.GetIsChangeAnimation())
		{
			this->isInitialize = false;
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["FLY_INTERVAL"]);
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossFlyAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	const VECTOR POSITION					= _boss.GetRigidbody().GetPosition();
	const VECTOR TARGET						= player.GetRigidbody().GetPosition();
	const VECTOR POSITION_TO_TARGET_VECTOR	= VSub(POSITION, TARGET); //���W�ƈړ��ڕW�Ԃ̃x�N�g��
	const float  DISTANCE					= VSize(POSITION_TO_TARGET_VECTOR);
	/*�ǉ�����~���l*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�C���^�[�o�����c���Ă�����~���l���O�ɂ���*/
	else if (!this->parameter->CalcInterval())
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
	else if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	{
		addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE * 2.0f;
	}
	else
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}
