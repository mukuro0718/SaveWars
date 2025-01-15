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
#include "BossRotateSlashAttack.h"
#include "BossRotateSlashAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRotateSlashAction::BossRotateSlashAction()
	: isClose(false)
{
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::ROTATE_SLASH);
	this->attack			= new BossRotateSlashAttack(ATTACK_TYPE);
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->rotateFrame		= json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_ROTATE_FRAME"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::ROTATE_SLASH);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_INTERVAL"];
	this->checkState		= static_cast<int>(Boss::BossState::ANGRY);
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->normalDesireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRotateSlashAction::~BossRotateSlashAction()
{

}

/// <summary>
/// ������
/// </summary>
void BossRotateSlashAction::Initialize()
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
void BossRotateSlashAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�U������*/
	{
		//����������Ă��Ȃ�������
		if (!this->isInitialize)
		{
			//�U���^�C�v�̐ݒ�
			_boss.SetAttackType(Boss::AttackType::SLASH_1);
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
			//�q�b�g�X�g�b�v�̐ݒ�
			this->hitStop->SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
			//�U���q�b�g�t���O������
			this->attack->OffIsHitAttack();
		}
		//�q�b�g�X�g�b�v���Ȃ瑁�����^�[��
		if (this->hitStop->IsHitStop()) return;
	}

	/*�J�E���g�̌v��*/
	this->frameCount++;

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();												//���W
		const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();												//�ړ��ڕW
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();												//��]��
			  VECTOR positonToTargetVector	= VSub(POSITION, MOVE_TARGET);												//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);														//����
			  float  speed					= 0.0f;																				//�ړ��X�s�[�h�̐ݒ�
			  bool   isRotation				= false;																			//��]���邩
		//���̍U���͍ŏ�����U������
		this->isAllowAction = true;
		{
			//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
			if (this->frameCount < this->rotateFrame)
			{
				isRotation = true;
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//��]����
		{
			if (isRotation)
			{
				//��]����⊮����
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->rotateLerpValue);
				//��]����ݒ�
				_boss.SetRotation(nowRotation);
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
void BossRotateSlashAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��Ԃ�NORMAL,ANGRY��������~���l�𑝉�����*/
	else if (_boss.GetAngryState() >= this->checkState)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//�ړ��ڕW
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE = VSize(POSITION_TO_TARGET);			//����

		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= this->actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{���O�������瑁�����^�[��
			if (_boss.GetAttackComboCount() == 0)return;
			//�O�̍U�����X���b�V���R���{�Q��������
			if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = this->maxDesireValue;
			}
			//����ȊO��������
			else
			{
				this->parameter->desireValue = this->normalDesireValue;
			}
		}
	}
}