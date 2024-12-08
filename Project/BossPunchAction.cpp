#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossPunchAttack.h"
#include "BossPunchAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossPunchAction::BossPunchAction()
{
	this->attack = new BossPunchAttack(static_cast<int>(BossAttack::AttackType::PUNCH));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossPunchAction::~BossPunchAction()
{

}

/// <summary>
/// ������
/// </summary>
void BossPunchAction::Initialize()
{
	this->isSelect = false;
	this->isInitialize = false;
	this->frameCount = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossPunchAction::Update(Boss& _boss)
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
			_boss.SetAttackType(Boss::AttackType::PUNCH);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�������t���O�𗧂Ă�
			this->isInitialize = true;
		}
	}

	/*�q�b�g�X�g�b�v�̍X�V*/
	if (this->attack->GetIsHitAttack())
	{
		const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::PUNCH);
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
		//�q�b�g�X�g�b�v���������瑁�����^�[��
		if (this->hitStop->IsHitStop()) return;
	}

	/*�J�E���g�̌v��*/
	this->frameCount++;

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();										//���W
		const VECTOR LERP_VALUE				= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);	//��]���̕⊮�l
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();										//��]��
			  VECTOR positonToTargetVector	= VSub(POSITION, this->moveTarget);											//�v���C���[���玩���̍��W�܂ł̃x�N�g��
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);													//����
			  float  speed					= 0.0f;																		//�ړ��X�s�[�h
			  bool   isRotation				= false;																	//��]���邩
		{
			//�t���[���J�E���g����]�萔�����������狖�t���O�𗧂Ă�
			if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
			{
				this->isAllowAction = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ��]�t���O�𗧂Ă�
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["PUNCH_ROTATE_FRAME"])
			{
				isRotation = true;
			}
			//�t���[���J�E���g���萔�ȓ��Ȃ�ړ��ڕW���X�V����
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["PUNCH_HOMING_FRAME"])
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
		////�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
		//if (!this->isAllowAction)return;
		//�ړ��x�N�g���̐ݒ�
		{
			//�ړ��ڕW�̐ݒ�
			_boss.SetNowMoveTarget(this->moveTarget);
			//��]�������ƂɈړ��x�N�g�����o��
			direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
			//�ړ��x�N�g���𐳋K��
			direction = VNorm(direction);
			//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
			VECTOR aimVelocity = VScale(direction, speed);							//�Z�o���ꂽ�ړ��x�N�g��
			VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				//�O�̈ړ��x�N�g��
			VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
			//�ړ��x�N�g���̐ݒ�
			_boss.SetVelocity(newVelocity);
		}
	}

	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::PUNCH));
		//�Đ����Ԃ̐ݒ�
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		//�A�j���[�V�����̍Đ�
		_boss.PlayAnimation();
	}



	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["PUNCH_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossPunchAction::CalcParameter(const Boss& _boss)
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
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::PUNCH)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//�R���{���O�������瑁�����^�[��
			if (_boss.GetAttackComboCount() == 0)return;
			//�O�̍U����STAB��������
			if (type == Boss::AttackType::STAB)
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