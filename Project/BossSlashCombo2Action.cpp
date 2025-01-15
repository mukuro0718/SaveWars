#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlashCombo2Attack.h"
#include "BossSlashCombo2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossSlashCombo2Action::BossSlashCombo2Action()
	:isClose(false)
{
	auto& json	= Singleton<JsonManager>::GetInstance();
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::SLASH_COMBO_2);
	this->attack			= new BossSlashCombo2Attack(ATTACK_TYPE);
	this->targetOffset		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_TARGET_OFFSET"];
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->rotateFixFrame	= json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"];
	this->moveFrame			= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_FRAME"];
	this->rotateFrame1		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME"];
	this->rotateFrame2		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME_2"];
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_SPEED"];
	this->stopDistance		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_STOP_MOVE_DISTANCE"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_COMBO_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_INTERVAL"];
	this->checkState		= static_cast<int>(Boss::BossState::ANGRY);
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->normalDisireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossSlashCombo2Action::~BossSlashCombo2Action()
{

}

/// <summary>
/// ������
/// </summary>
void BossSlashCombo2Action::Initialize()
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
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlashCombo2Action::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();

	/*�U������*/
	{
		//����������Ă��Ȃ�������
		if (!this->isInitialize)
		{
			//�U���^�C�v�̐ݒ�
			_boss.SetAttackType(Boss::AttackType::SLASH_COMBO_2);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�ړ��ڕW�̐ݒ�
			const VECTOR TARGET = player.GetRigidbody().GetPosition();
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition();
			VECTOR positonToTargetVector = VSub(POSITION, TARGET);
			VECTOR targetOffset = VNorm(VCross(VNorm(positonToTargetVector), Gori::UP_VEC));
			this->moveTarget = VAdd(TARGET, VScale(targetOffset, this->targetOffset));
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
			//�q�b�g�X�g�b�v�̐ݒ�
			this->hitStop->SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
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
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//���W
			  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//��]��
			  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR direction   = VGet(0.0f, 0.0f, 0.0f);				//����
			  float  speed		 = 0.0f;								//�ړ��X�s�[�h�̐ݒ�
			  bool   isRotation  = false;								//��]���邩
			  bool   isMove		 = false;								//�ړ����邩
		{
			//�t���[���J�E���g���萔�����������狖�t���O�𗧂Ă�
			if (this->frameCount > this->rotateFixFrame)
			{
				this->isAllowAction = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
			if (this->frameCount < this->moveFrame)
			{
				isMove = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
			if (this->frameCount < this->rotateFrame1 || 
				this->frameCount >= this->rotateFrame2)
			{
				isRotation = true;
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
			////�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
			//if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_HOMING_FRAME"] ||
			//	this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME_2"])
			//{
			//}
		}
		//��]����
		{
			if (isRotation)
			{
				//��]����⊮����
				nowRotation = GetLerpRotation(_boss, toTarget, nowRotation, this->rotateLerpValue);
				//��]����ݒ�
				_boss.SetRotation(nowRotation);
			}
		}
		////�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		//if (!this->isAllowAction)return;
		//�ړ����x�̐ݒ�
		{
			//��x����苗���ɋ߂Â��Ă��Ȃ� && �ړ��t���O�������Ă�����
			if (!this->isClose && isMove)
			{
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(toTarget);
				//�������萔�ȏ�Ȃ瑬�x��ݒ肷��
				if (DISTANCE >= this->stopDistance)
				{
					speed = this->speed;
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
	}

	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(this->nextAnimation);
		//�Đ����Ԃ̐ݒ�
		_boss.SetAnimationPlayTime(this->animationPlayTime);
		//�A�j���[�V�����̍Đ�
		_boss.PlayAnimation();
	}

	/*�U������̍X�V*/
	this->attack->Update(0.0f);

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(this->maxInterval);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlashCombo2Action::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��Ԃ�ANGRY��������~���l�𑝉�����*/
	else if (_boss.GetAngryState() >= this->checkState)
	{
		/*���������߂�*/
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//�ړ��ڕW
		const VECTOR TO_TARGET	 = VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE	 = VSize(TO_TARGET);					//����

		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= this->actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{�����c���Ă��Ȃ������瑁�����^�[��
			if (_boss.GetAttackComboCount() == 0) return;
			//�O�̍U�����X���b�V���R���{�Q��������
			if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = this->maxDesireValue;
			}
			//����ȊO��������
			else
			{
				this->parameter->desireValue = this->normalDisireValue;
			}
		}
	}
}