#include <DxLib.h>
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "BossChase.h"
#include "PlayerManager.h"

BossChase::BossChase()
{

}

BossChase::~BossChase()
{

}

void BossChase::Initialize()
{
}

void BossChase::Finalize()
{

}

Rigidbody& BossChase::Update(Character& _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR TARGET		  = player.GetRigidbody().GetPosition();	//�ڕW
	const VECTOR position	  = _boss.GetRigidbody().GetPosition();	//���W
		  VECTOR rotation	  = _boss.GetRigidbody().GetRotation();	//��]��
		  VECTOR nextRotation = VGet(0.0f, 0.0f, 0.0f);
		  Rigidbody out;

	/*��]���̐ݒ�*/
	//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
	VECTOR positonToTargetVector = VSub(position, TARGET);
	//�A�^���Q���g���Ċp�x�����߂�
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	//���̉�]���ւ�[�Ղ��g���ĉ�]����
	rotation = _boss.Lerp(rotation, nextRotation, VGet(0.1f, 0.1f, 0.1f));
	//��]����ݒ�
	out.SetRotation(rotation);

	/*�ړ����x�̐ݒ�*/
	//���̃N���X�͕����Ă���Ƃ��ɌĂ΂��͂��Ȃ̂ŁA�����̃X�s�[�h������
	float speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];

	/*�ړ��ʂ��o��*/
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);
	//��]�������ƂɈړ��������o��
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
	//�����x�N�g���𐳋K��
	direction = VNorm(direction);
	//�d�͂��l�����邽�߁AY�̂ݑO�̈ړ��x�N�g�����Q�Ƃ���
	VECTOR aimVelocity = VScale(direction, speed);
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
	//�ړ��x�N�g���̐ݒ�
	out.SetVelocity(newVelocity);

	return out;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossChase::CalcParameter(const int _hp, const int _angryValue, const float _distance)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ǉ�����~���l*/
	int addDesireValue = 0;

	/*����HP���O�ȉ���������~���l���O�ɂ���*/
	if (_hp <= 0)
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȏ㗣��Ă�����~���l��{��������*/
	else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}
	else
	{
		addDesireValue = 1;
	}

	/*�~���l�𑝉�������*/
	this->parameter->AddDesireValue(addDesireValue);
}