#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
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
{
	this->attack = new BossSlashAttack(static_cast<int>(BossAttack::AttackType::SLASH));
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

	/*�A�j���[�V�����̐ݒ�*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH));

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json	 = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition(); //���W
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();//�ړ��ڕW
	VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //��]��
	VECTOR positonToTargetVector = VSub(POSITION, MOVE_TARGET); //���W�ƈړ��ڕW�Ԃ̃x�N�g��
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);

	/*�q�b�g�X�g�b�v�̍X�V*/
	if (this->attack->GetIsHitAttack())
	{
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

		this->hitStop->SetHitStop
		(
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][static_cast<int>(BossAttack::AttackType::SLASH)],
			static_cast<int>(HitStop::Type::STOP),
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][static_cast<int>(BossAttack::AttackType::SLASH)],
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][static_cast<int>(BossAttack::AttackType::SLASH)]
		);
		this->attack->OffIsHitAttack();
	}
	if (this->hitStop->IsHitStop()) return;

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetNowMoveTarget(MOVE_TARGET);

	/*����������Ă��Ȃ�������*/
	if (!this->isInitialize)
	{
		//�G�t�F�N�g�𗧂Ă�
		effect.OnIsEffect(EffectManager::EffectType::BOSS_SLASH);
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
		this->isInitialize = true;
	}

	/*�J�E���g�̌v��*/
	bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_SLOW_FRAME_COUNT"]);

	/*��]����*/
	{
		if (!isEndCount)
		{
			//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
			//�A�[�N�^���W�F���g���g���Ċp�x�����߂�
			nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
			//��]������
			_boss.SetRotation(nowRotation);
		}
	}

	/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
	{
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*�A�j���[�V�����̍Đ�*/
		_boss.PlayAnimation();
	}

	/*�ړ��X�s�[�h�̐ݒ�*/
	float speed = 0.0f;

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

	/*�U������̍X�V*/
	this->attack->Update();

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"]);
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

	/*�C���^�[�o�����c���Ă�����~���l���O�ɂ���*/
	if (this->parameter->interval < 0)
	{
		this->parameter->interval--;
		this->parameter->desireValue = 0;
	}

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	else if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		this->parameter->desireValue = 0;
	}

	/*Phase��2�ȏゾ������~���l�𑝉�����*/
	else if (_boss.GetNowPhase() >= static_cast<int>(Boss::Phase::PHASE_1))
	{
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
		{
			this->parameter->desireValue += json.GetJson(JsonManager::FileType::ENEMY)["ADD_SLASH_VALUE"];
		}
		else
		{
			this->parameter->desireValue = 0;
		}
	}

}