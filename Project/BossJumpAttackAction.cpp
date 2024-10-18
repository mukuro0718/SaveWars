#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "BossJumpAttackAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossJumpAttackAction::BossJumpAttackAction()
{
	this->attack = new BossJumpAttack(static_cast<int>(BossAttack::AttackType::JUMP_ATTACK));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossJumpAttackAction::~BossJumpAttackAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossJumpAttackAction::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossJumpAttackAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::JUMP_ATTACK));

	/*�U���^�C�v�̐ݒ�*/
	const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::JUMP_ATTACK);
	_boss.SetAttackType(Boss::AttackType::JUMP_ATTACK);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�q�b�g�X�g�b�v�̍X�V*/
	if (this->attack->GetIsHitAttack())
	{
		this->hitStop->SetHitStop
		(
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE],
			static_cast<int>(HitStop::Type::STOP),
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE],
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE]
		);
		this->attack->OffIsHitAttack();
	}
	if (this->hitStop->IsHitStop()) return;



	/*����������Ă��Ȃ�������*/
	if (!this->isInitialize)
	{
		//�G�t�F�N�g�𗧂Ă�
		//effect.();
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
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
		float speed = 0.0f;

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

	/*�A�j���[�V�����̍Đ�*/
	{
		float animationPlayTime = _boss.GetAnimationPlayTime();
		//�Đ����Ԃ̐ݒ�
		_boss.SetAnimationPlayTime(animationPlayTime);
		//�Đ�
		_boss.PlayAnimation();
	}

	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["FLY_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossJumpAttackAction::CalcParameter(const Boss& _boss)
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
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		return;
	}

	/*Phase��8�ȏゾ������~���l�𑝉�����*/
	else if (_boss.GetNowPhase() >= static_cast<int>(Boss::Phase::PHASE_8))
	{
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::JUMP_ATTACK)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			if (_boss.GetAttackComboCount() == 0)
			{
				this->parameter->desireValue = 1;
			}
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}

}
