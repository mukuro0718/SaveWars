#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossSlashAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossSlashAction::BossSlashAction()
	: isClose(false)
{
	this->attack = new BossSlashAttack(static_cast<int>(BossAttack::AttackType::SLASH_1));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossSlashAction::~BossSlashAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossSlashAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->isAllowAction			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlashAction::Update(Boss& _boss)
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
			_boss.SetAttackType(Boss::AttackType::SLASH_1);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�ړ��ڕW�̐ݒ�
			const VECTOR TARGET = player.GetRigidbody().GetPosition();
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition();										//���W
			VECTOR positonToTargetVector = VSub(POSITION, TARGET);											//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			VECTOR targetOffset = VNorm(VCross( VNorm(positonToTargetVector), Gori::UP_VEC));
			this->moveTarget = VAdd(TARGET, VScale(targetOffset, json.GetJson(JsonManager::FileType::ENEMY)["SLASH_TARGET_OFFSET"]));
			//�������t���O�𗧂Ă�
			this->isInitialize = true;
		}
	}

	/*�q�b�g�X�g�b�v*/
	{
		//�U�����������Ă�����
		if (this->attack->GetIsHitAttack())
		{
			//�U���̎��
			const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::SLASH_1);
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
		//�q�b�g�X�g�b�v�����Ă����瑁�����^�[��
		if (this->hitStop->IsHitStop()) return;
	}


	/*�J�E���g�̌v��*/
	this->frameCount++;

	/*�ړ�����*/
	{
		//�g�p����ϐ�
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();										//���W
		const VECTOR LERP_VALUE				= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//��]���̕⊮�l
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();										//��]��
			  VECTOR positonToTargetVector	= VSub(POSITION, this->moveTarget);											//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  float  speed					= 0.0f;																		//�ړ��X�s�[�h�̐ݒ�
			  bool   isRotation				= false;																	//��]���邩
			  bool   isMove					= false;																	//�ړ����邩
		{
			//�t���[���J�E���g���萔�����������狖�t���O�𗧂Ă�
			if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
			{
				this->isAllowAction = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_MOVE_FRAME"])
			{
				isMove = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_ROTATE_FRAME"])
			{
				isRotation = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_HOMING_FRAME"])
			{
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//��]����
		{
			if (isRotation)
			{
				//��]����⊮����
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, LERP_VALUE);
				//��]����ݒ�
				_boss.SetRotation(nowRotation);
			}
		}
		//�ړ����x�̐ݒ�
		{
			//��x����苗���ɋ߂Â��Ă��Ȃ� && �ړ��t���O�������Ă����� && �A�N�V�������t���O�������Ă�����
			if (!this->isClose && isMove && this->isAllowAction)
			{
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(positonToTargetVector);
				//�������萔�ȏ�Ȃ瑬�x��ݒ肷��
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_MOVE_SPEED"];
				}
				//��薢���Ȃ�t���O�𗧂Ă�
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
			//�V�����ړ��x�N�g���̎Z�o
			VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
			//�ړ��x�N�g���̐ݒ�
			_boss.SetVelocity(newVelocity);
		}
		////�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		//if (!this->isAllowAction)return;
	}

	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH_1));
		//�Đ����Ԃ̐ݒ�
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		//�A�j���[�V�����̍Đ�
		_boss.PlayAnimation();
	}

	/*�U������*/
	this->attack->Update();

	/*�I������*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (_boss.GetIsChangeAnimation())
		{
			//������/�߂Â������𔻒肷��t���O������
			this->isInitialize = false;
			this->isClose = false;
			//���̑��ϐ��̏������ƃC���^�[�o���̃Z�b�g
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"]);
			//�R���{�������炷
			_boss.DecAttackComboCount();
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlashAction::CalcParameter(const Boss& _boss)
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
	if ((_boss.GetHP() <= 0))
	{
		return;
	}

	/*��Ԃ�TIRED,NORMAL,ANGRY��������~���l�𑝉�����*/
	if (_boss.GetAngryState() >= static_cast<int>(Boss::AngryStateType::NORMAL))
	{

		/*�������萔�ȓ���������~���l��ʏ�ɂ���*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::SLASH_1)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{���O�������瑁�����^�[��
			if (_boss.GetAttackComboCount() == 0)return;
			//�O�̍U������]�U���܂��̓p���`��������
			if (type == Boss::AttackType::ROTATE_SLASH || type == Boss::AttackType::PUNCH)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			}
			//����ȊO��������
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}