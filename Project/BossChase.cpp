#include <DxLib.h>
#include "UseJson.h"
#include "BossAction.h"
#include "GoriLib.h"
#include "Boss.h"
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

void BossChase::Update(Boss* _boss)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR TARGET		  = player.GetPosition();	//�ڕW
	const VECTOR position	  = _boss->GetPosition();	//���W
		  VECTOR rotation	  = _boss->GetRotation();	//��]��
		  VECTOR nextRotation = VGet(0.0f, 0.0f, 0.0f);

	/*��]���̐ݒ�*/
	//�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��
	VECTOR positonToTargetVector = VSub(position, TARGET);
	//�A�^���Q���g���Ċp�x�����߂�
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	//���̉�]���ւ�[�Ղ��g���ĉ�]����
	rotation = _boss->Lerp(rotation, nextRotation, VGet(0.1f, 0.1f, 0.1f));
	//��]����ݒ�
	_boss->SetRotation(rotation);

	/*�ړ����x�̐ݒ�*/
	//���̃N���X�͕����Ă���Ƃ��ɌĂ΂��͂��Ȃ̂ŁA�����̃X�s�[�h������
	float speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	//�X�s�[�h�̐ݒ�
	_boss->SetSpeed(speed);

	/*�ړ��ʂ��o��*/
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);
	//��]�������ƂɈړ��������o��
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
	//�����x�N�g���𐳋K��
	direction = VNorm(direction);
	//�d�͂��l�����邽�߁AY�̂ݑO�̈ړ��x�N�g�����Q�Ƃ���
	VECTOR aimVelocity = VScale(direction, speed);
	VECTOR prevVelocity = _boss->GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
	//�ړ��x�N�g���̐ݒ�
	_boss->SetVelocity(newVelocity);
}