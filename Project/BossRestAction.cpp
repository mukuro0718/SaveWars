#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRestAction::BossRestAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRestAction::~BossRestAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossRestAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
		auto& json = Singleton<JsonManager>::GetInstance();

		/*�g�p����l�̏���*/
		const float  SPEED		= 0.0f;
		const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();
			  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);			 //����

		/*�X�s�[�h���O�ɂ���*/
		_boss.SetSpeed(SPEED);

		/*��]�������ƂɁA�ړ�����������o��*/
		direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));

		/*�����x�N�g���𐳋K��*/
		direction = VNorm(direction);

		/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
		VECTOR aimVelocity = VScale(direction, SPEED);								//�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��

		/*�ړ��x�N�g���̐ݒ�*/
		_boss.SetVelocity(newVelocity);

		//�t���[���v��
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
		//�t���[���v�����I�����Ă�����
		if (isEndCount)
		{
			OffIsSelect(0);
		}
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	/*�ǉ�����~���l*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HP���O�ȉ��܂��̓t�F�[�Y���قȂ��Ă�����~���l��0�ɂ���*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	///*�{��̃Q�[�W�ɔ�Ⴕ�āA�~���Q�[�W�����߂�*/
	//else
	//{
	//	this->parameter->desireValue = this->parameter->MAX_PARAMETER - _angryValue;
	//}
	this->parameter->AddDesireValue(addDesireValue);
}