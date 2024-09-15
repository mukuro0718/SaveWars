#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossHurricaneKickAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "BossAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossHurricaneKickAction::BossHurricaneKickAction()
{
	this->isInitialize = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossHurricaneKickAction::~BossHurricaneKickAction()
{

}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossHurricaneKickAction::Update(Boss& _boss)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& effect = Singleton<EffectManager>::GetInstance();
		auto& attack = Singleton<BossAttackManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();

		/*�g�p����l�̏���*/
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition(); //���W
		const VECTOR NEXT_MOVE_TARGET		= player.GetRigidbody().GetPosition();
			  VECTOR nowMoveTarget			= _boss.GetNowMoveTarget();//�ړ��ڕW
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation(); //��]��
			  VECTOR positonToTargetVector	= VGet(0.0f, 0.0f, 0.0f); //�v���C���[���玩���̍��W�܂ł̃x�N�g��
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);

		/*����������Ă��Ȃ�������*/
		if (!this->isInitialize)
		{
			//�G�t�F�N�g�𗧂Ă�
			//effect.OnIsBossSlashEffect();
			//�U���t���O�𗧂Ă�
			attack.OnIsStart(static_cast<int>(BossAttackManager::AttackType::HURRICANE_KICK));
			this->isInitialize = true;
			_boss.SetAnimationPlayTime(0.0f);
		}

		/*�J�E���g�̌v��*/
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_ADD_FRAME_COUNT"]);
		
		/*�ړ��ڕW*/
		if (isEndCount)
		{
			//���݂̈ړ��x�N�g�����o��
			/*nowMoveTarget = _boss.Lerp(nowMoveTarget, NEXT_MOVE_TARGET, VGet(0.01f, 0.01f, 0.01f));*/
			//�ړ��x�N�g���̐ݒ�
			_boss.SetNowMoveTarget(NEXT_MOVE_TARGET);
			//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
			positonToTargetVector = VSub(POSITION, nowMoveTarget); 
		}

		/*��]����*/
		if (isEndCount)
		{
			//�A�[�N�^���W�F���g���g���Ċp�x�����߂�
			nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
			//��]������
			_boss.SetRotation(nowRotation);
		}

		/*�A�j���[�V�����Đ����Ԃ̐ݒ�*/
		{
			float animationPlayTime = _boss.GetNowAnimationPlayTime();
			//�J�E���g���I�����Ă��Ȃ����
			animationPlayTime += static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_ADD_PLAY_TIME"]);
			if (animationPlayTime >= static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MAX_PLAY_TIME"]))
			{
				animationPlayTime = static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MAX_PLAY_TIME"]);
			}
			_boss.SetAnimationPlayTime(animationPlayTime);
		}

		/*�ړ��X�s�[�h�̐ݒ�*/
		float speed = 0.0f;
		{
			speed = json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MOVE_SPEED"];
		}

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

		/*�I������*/
		if (_boss.GetIsChangeAnimation())
		{
			this->isInitialize = false;
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_INTERVAL"]);
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossHurricaneKickAction::CalcParameter(const Boss& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ǉ�����~���l*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�C���^�[�o���̐ݒ�*/
	//this->parameter->SetInterval(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"]);

	///*�C���^�[�o�����c���Ă�����~���l���O�ɂ���*/
	//if (!this->parameter->CalcInterval()) addDesireValue = -this->parameter->MAX_PARAMETER;

	///*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
	//else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE * 2;
	//}
	//else
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;
	//}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}