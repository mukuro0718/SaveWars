#include <DxLib.h>
#include "EffekseerForDXLib.h"
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
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlash2Attack.h"
#include "BossSlash2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossSlash2Action::BossSlash2Action()
	: isClose(false)
{
	this->attack = new BossSlash2Attack(static_cast<int>(BossAttack::AttackType::SLASH_2));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossSlash2Action::~BossSlash2Action()
{

}

/// <summary>
/// ������
/// </summary>
void BossSlash2Action::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void BossSlash2Action::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }


	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�U������*/
	{
		//����������Ă��Ȃ�������
		if (!this->isInitialize)
		{
			//�U���^�C�v�̐ݒ�
			_boss.SetAttackType(Boss::AttackType::SLASH_2);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�ړ��ڕW�̐ݒ�
			this->moveTarget = player.GetRigidbody().GetPosition();
			//�������t���O�𗧂Ă�
			this->isInitialize = true;
		}

	}

	/*�q�b�g�X�g�b�v*/
	{
		//�q�b�g�X�g�b�v�̍X�V
		if (this->attack->GetIsHitAttack())
		{
			const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::SLASH_2);
			//�q�b�g�X�g�b�v�̐ݒ�
			this->hitStop->SetHitStop
			(
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE],
				static_cast<int>(HitStop::Type::STOP),
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE],
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE]
			);
			//�U���q�b�g�t���O������
			this->attack->OffIsHitAttack();
		}
		/*�q�b�g�X�g�b�v�����Ă����瑁�����^�[��*/
		if (this->hitStop->IsHitStop()) return;
	}

	/*�J�E���g�̌v��*/
	this->frameCount++;

	/*�ړ�����*/
	{
		/*�g�p����l�̏���*/
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//��]���̕⊮�l
		VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//��]��
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);			//����
		float  speed = 0.0f;									//�ړ��̑���
		bool	 isRotation = false;								//��]���邩
		bool   isMove = false;								//�ړ����邩

		{
			//�t���[���J�E���g����]�萔�����������狖�t���O�𗧂Ă�
			if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
			{
				isAllowAction = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_FRAME"])
			{
				isMove = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_ROTATE_FRAME"])
			{
				isRotation = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_HOMING_FRAME"])
			{
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//��]����
		{
			//��]�t���O�������Ă�����
			if (isRotation)
			{
				//���W�ƈړ��ڕW�Ԃ̃x�N�g��
				VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);
				//��]����⊮����
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, LERP_VALUE);
				//��]������
				_boss.SetRotation(nowRotation);
			}
		}
		////�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		//if (!this->isAllowAction)return;
		//�ړ��X�s�[�h�̐ݒ�
		{
			//��x�ł��߂Â��Ă��Ȃ������ړ��t���O�������Ă�����
			if (!this->isClose && isMove)
			{
				//���W�ƈړ��ڕW�Ԃ̃x�N�g��
				VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(positonToTargetVector);
				//�������萔�ȏゾ������ړ����x��������
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_SPEED"];
				}
				//������������������t���O�𗧂Ă�
				else
				{
					this->isClose = true;
				}
			}
		}
		//�ړ��x�N�g���̐ݒ�
		{
			//�ړ��ڕW�̐ݒ�
			_boss.SetNowMoveTarget(this->moveTarget);
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
	}






	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH_2));
		//�A�j���[�V�����Đ�����
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		//�A�j���[�V�����̍Đ�
		_boss.PlayAnimation();
	}

	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (_boss.GetIsChangeAnimation())
		{
			//�e�t���O������
			this->isInitialize = false;
			this->isClose = false;
			//���̑��ϐ��̏������ƃC���^�[�o���̃Z�b�g
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_INTERVAL"]);
			//�R���{�������炷
			_boss.DecAttackComboCount();
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlash2Action::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���������߂�*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//�ړ��ڕW
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
	const float  DISTANCE = VSize(POSITION_TO_TARGET);			//����

	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��Ԃ�TIRED,NORMAL,ANGRY��������~���l�𑝉�����*/
	else if (_boss.GetAngryState() >= static_cast<int>(Boss::BossState::NORMAL))
	{
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::SLASH_2)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{�����c���Ă��Ȃ������烊�^�[��
			if (_boss.GetAttackComboCount() == 0)return;
			//�O�̍U�����X���b�V���R���{�Q��������
			if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			}
			//����ȊO�Ȃ�
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}