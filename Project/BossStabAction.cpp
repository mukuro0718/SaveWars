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
#include "BossStabAttack.h"
#include "BossStabAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossStabAction::BossStabAction()
	: isClose(false)
{
	this->attack = new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossStabAction::~BossStabAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossStabAction::Initialize()
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
void BossStabAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json	 = Singleton<JsonManager>::GetInstance();

	/*�U������*/
	{
		//����������Ă��Ȃ�������
		if (!this->isInitialize)
		{
			//�U���^�C�v�̐ݒ�
			_boss.SetAttackType(Boss::AttackType::STAB);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�������t���O�𗧂Ă�
			this->isInitialize = true;
		}
	}

	/*�q�b�g�X�g�b�v�̍X�V*/
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
		//�q�b�g�X�g�b�v���������瑁�����^�[��
		if (this->hitStop->IsHitStop()) return;
	}

	/*�J�E���g�̌v��*/
	this->frameCount++;
	bool isRotation = false;	//��]���邩
	bool isMove		= false;	//�ړ����邩
	bool isStop		= false;	//��~���邩
	{
			//�t���[���J�E���g���萔�����������狖�t���O�𗧂Ă�
		if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
		{
			this->isAllowAction = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_FRAME"])
		{
			isMove = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
		else if (this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_FRAME"])
		{
			isStop = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_FRAME"])
		{
			isRotation = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_HOMING_FRAME"])
		{
			this->moveTarget = player.GetRigidbody().GetPosition();
		}
	}

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//��]���̕⊮�l
		VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//��]��
		VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);			//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);				//����
		float  speed = 0.0f;																		//�ړ��X�s�[�h�̐ݒ�
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
		//�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		if (!this->isAllowAction)return;
		//�ړ����x�̐ݒ�
		{
			//��x�ł��߂Â��Ă��Ȃ������ړ��t���O�������Ă��Ȃ�������
			if (!this->isClose && !isMove && !isStop)
			{
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(positonToTargetVector);
				//�������萔�ȏォ
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
				}
				else
				{
					//�߂Â����t���O�𗧂Ă�
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

	}

	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::STAB));
		//�A�j���[�V�����Đ�����
		float animationPlayTime = _boss.GetAnimationPlayTime();
		//�J�E���g���I�����Ă��Ȃ���΃A�j���[�V�����Đ����x��x������
		if (isMove)
		{
			animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"];
		}
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*�A�j���[�V�����̍Đ�*/
		_boss.PlayAnimation();
	}

	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossStabAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���������߂�*/
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//�ړ��ڕW
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//����

	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��Ԃ�ANGRY��������~���l�𑝉�����*/
	else if (_boss.GetAngryState() >= static_cast<int>(Boss::BossState::ANGRY))
	{
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::STAB)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{�����c���Ă�����
			if (_boss.GetAttackComboCount() != 0)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}