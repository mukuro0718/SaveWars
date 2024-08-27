#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "Player.h"
#include "Vector4.h"
#include "Collider.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerManager::PlayerManager()
	:player(nullptr)
{
	this->player = new Player();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerManager::~PlayerManager()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerManager::Initialize()
{
	this->player->Initialize();
}

/// <summary>
/// �A�N�V����
/// </summary>
void PlayerManager::Action()
{
	this->player->Action();
}

/// <summary>
/// �X�V
/// </summary>
void PlayerManager::Update()
{
	this->player->Update();
}
/// <summary>
/// �`��
/// </summary>
const void PlayerManager::Draw()const
{
	this->player->Draw();
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
void PlayerManager::FixMoveVector(const VECTOR _fixVector)
{
	this->player->FixMoveVector(_fixVector);
}

/// <summary>
/// ���W�̎擾
/// </summary>
const VECTOR PlayerManager::GetPosition()const
{
	return this->player->GetPosition();
}

/// <summary>
/// �����Ă�������̎擾
/// </summary>
const VECTOR PlayerManager::GetDirection()const
{
	return this->player->GetDirection();
}

/// <summary>
/// �ړ��t���O�̎擾
/// </summary>
const bool PlayerManager::IsMove()const
{
	return this->player->IsMove();
}
/// <summary>
/// �_���[�W�̎擾
/// </summary>
const int PlayerManager::GetDamage()const
{
	return this->player->GetDamage();
}
/// <summary>
/// �_���[�W����
/// </summary>
void PlayerManager::CalcDamage(const int _damage)
{
	return this->player->CalcDamage(_damage);
}

/// <summary>
/// �V���b�g�t���O
/// </summary>
const bool PlayerManager::IsAttack()const
{
	return this->player->IsAttack();
}

const Collider PlayerManager::GetCharacterCollider()
{
	return this->player->GetCharacterCollider();
}
const Collider PlayerManager::GetAttackCollider()
{
	return this->player->GetAttackCollider();
}
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const int PlayerManager::GetAttackNumber()const
{
	return this->player->GetAttackNumber();
}
const int PlayerManager::GetHitNumber()const
{
	return this->player->GetHitNumber();
}
void PlayerManager::SetHitNumber(const int _attackNumber)
{
	this->player->SetHitNumber(_attackNumber);
}