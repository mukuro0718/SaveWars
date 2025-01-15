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

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossStabAction::BossStabAction()
	: isClose(false)
{
	this->attack			= new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
	auto& json				= Singleton<JsonManager>::GetInstance();
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::STAB);
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->moveFrame			= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_FRAME"];
	this->stopFrame			= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_FRAME"];
	this->rotateFrame		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_FRAME"];
	this->rotateFixFrame	= json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"];
	this->homingFrame		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_HOMING_FRAME"];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->stopDistance		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"];
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::STAB);
	this->slowPlayTime		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"];
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->normalDisireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
	this->checkState		= static_cast<int>(Boss::BossState::ANGRY);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
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
			//�A�j���[�V�����̐ݒ�
			_boss.SetNowAnimation(this->nextAnimation);
		}
	}

	/*�q�b�g�X�g�b�v�̍X�V*/
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
		if (this->frameCount > this->rotateFixFrame)
		{
			this->isAllowAction = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
		if (this->frameCount < this->moveFrame)
		{
			isMove = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
		else if (this->frameCount >= this->stopFrame)
		{
			isStop = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
		if (this->frameCount < this->rotateFrame)
		{
			isRotation = true;
		}
		//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
		if (this->frameCount < this->homingFrame)
		{
			this->moveTarget = player.GetRigidbody().GetPosition();
		}
	}

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//���W
			  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//��]��
			  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR direction	 = VGet(0.0f, 0.0f, 0.0f);				//����
			  float  speed		 = 0.0f;								//�ړ��X�s�[�h�̐ݒ�
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
		//�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		if (!this->isAllowAction)return;
		//�ړ����x�̐ݒ�
		{
			//��x�ł��߂Â��Ă��Ȃ������ړ��t���O�������Ă��Ȃ�������
			if (!this->isClose && !isMove && !isStop)
			{
				//���W�ƈړ��ڕW�Ƃ̋��������߂�
				const float DISTANCE = VSize(toTarget);
				//�������萔�ȏォ
				if (DISTANCE >= this->stopDistance)
				{
					speed = this->speed;
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
		//�A�j���[�V�����Đ�����
		float animationPlayTime = this->animationPlayTime;
		//�J�E���g���I�����Ă��Ȃ���΃A�j���[�V�����Đ����x��x������
		if (isMove)
		{
			animationPlayTime = this->slowPlayTime;
		}
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*�A�j���[�V�����̍Đ�*/
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
void BossStabAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���������߂�*/
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//�ړ��ڕW
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//����

	this->parameter->desireValue = 0;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��Ԃ�ANGRY��������~���l�𑝉�����*/
	else if (_boss.GetAngryState() >= this->checkState)
	{
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE >= actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{�����c���Ă�����
			if (_boss.GetAttackComboCount() != 0)
			{
				this->parameter->desireValue = this->normalDisireValue;
			}
		}
	}
}