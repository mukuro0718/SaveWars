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
	this->attack = new BossSlashCombo2Attack(static_cast<int>(BossAttack::AttackType::SLASH_COMBO_2));
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

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH_COMBO_2));

	/*�U���^�C�v�̐ݒ�*/
	const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::STAB);
	_boss.SetAttackType(Boss::AttackType::SLASH_COMBO_2);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();	//�ړ��ڕW
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();	//��]��
		  VECTOR positonToTargetVector  = VSub(POSITION, MOVE_TARGET);			//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);				//����


	/*�ړ��x�N�g���̐ݒ�*/
	 _boss.SetNowMoveTarget(MOVE_TARGET);

	/*�q�b�g�X�g�b�v�̍X�V*/
	if (this->attack->GetIsHitAttack())
	{
		//auto& effect = Singleton<EffectManager>::GetInstance();
		//effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

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
		//effect.OnIsBossSlashEffect();
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
		this->isInitialize = true;
	}

	/*�J�E���g�̌v��*/
	bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_SLOW_FRAME_COUNT"]);

	/*�A�j���[�V��������*/
	{
		//�A�j���[�V�����Đ�����
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*�A�j���[�V�����̍Đ�*/
		_boss.PlayAnimation();
	}

	/*�ړ��X�s�[�h�̐ݒ�*/
	float speed = 0.0f;
	//��x�ł��߂Â��Ă��Ȃ�������
	if (!this->isClose)
	{
		//�J�E���g���I�����Ă�����
		if (!isEndCount)
		{
			//���W�ƈړ��ڕW�Ƃ̋��������߂�
			const float DISTANCE = VSize(positonToTargetVector);
			//�������萔�ȏォ
			if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_STOP_MOVE_DISTANCE"])
			{
				speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_SPEED"];
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
		VECTOR aimVelocity  = VScale(direction, speed);							 //�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //�O�̈ړ��x�N�g��
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
		//�ړ��x�N�g���̐ݒ�
		_boss.SetVelocity(newVelocity);
	}

	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_INTERVAL"]);
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
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::SLASH_COMBO_2)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			if (_boss.GetAttackComboCount() == 0)
			{
				this->parameter->desireValue = 1;
			}
			else if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			}
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}