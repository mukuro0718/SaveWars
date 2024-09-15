#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossRotatePunchAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "BossAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRotatePunchAction::BossRotatePunchAction()
	: isClose(false)
{
	this->isInitialize = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRotatePunchAction::~BossRotatePunchAction()
{

}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRotatePunchAction::Update(Boss& _boss)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& effect = Singleton<EffectManager>::GetInstance();
		auto& attack = Singleton<BossAttackManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();

		/*�g�p����l�̏���*/
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition(); //���W
		const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();//�ړ��ڕW
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation(); //��]��
			  VECTOR positonToTargetVector	= VSub(POSITION, MOVE_TARGET); //���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);

		/*�ړ��x�N�g���̐ݒ�*/
		_boss.SetNowMoveTarget(MOVE_TARGET);

		/*��]����*/
		{
			//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
			//�A�[�N�^���W�F���g���g���Ċp�x�����߂�
			nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
			//��]������
			_boss.SetRotation(nowRotation);
		}

		/*����������Ă��Ȃ�������*/
		if (!this->isInitialize)
		{
			//�G�t�F�N�g�𗧂Ă�
			//effect.OnIsBossRotatePunchEffect();
			//�U���t���O�𗧂Ă�
			attack.OnIsStart(static_cast<int>(BossAttackManager::AttackType::ROTATE_PUNCH));
			this->isInitialize = true;
		}

		/*�J�E���g�̌v��*/
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_SLOW_FRAME_COUNT"]);

		/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
		{
			float animationPlayTime = _boss.GetAnimationPlayTime();
			//�J�E���g���I�����Ă��Ȃ����
			if (!isEndCount)
			{
				animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_SLOW_PLAY_TIME"];
			}
			_boss.SetAnimationPlayTime(animationPlayTime);
		}

		/*�ړ��X�s�[�h�̐ݒ�*/
		float speed = 0.0f;
		if (!this->isClose)
		{
			//�J�E���g���I�����Ă�����
			if (!isEndCount)
			{
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(positonToTargetVector);
				//�������萔�ȏォ
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_MOVE_SPEED"];
				}
				else
				{
					this->isClose = true;
				}
			}
		}

		/*�ړ��x�N�g�����o��*/
		{
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

		/*�I������*/
		if (_boss.GetIsChangeAnimation())
		{
			this->isInitialize = false;
			this->isClose = false;
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_INTERVAL"]);
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRotatePunchAction::CalcParameter(const Boss& _boss)
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

	/*�C���^�[�o���̐ݒ�*/
	//this->parameter->SetInterval(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"]);

	///*�C���^�[�o�����c���Ă�����~���l���O�ɂ���*/
	//if (!this->parameter->CalcInterval()) addDesireValue = -this->parameter->MAX_PARAMETER;

	///*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
	//else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE * 2;
	//}
	//else
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;
	//}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}