#include <DxLib.h>
#include "UseSTL.h"
#include "Boss.h"
#include "Vector4.h"
#include "Collider.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyManager::EnemyManager()
	:boss(nullptr)
{
	this->boss = new Boss();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyManager::~EnemyManager()
{

}

/// <summary>
/// ������
/// </summary>
void EnemyManager::Initialize()
{
	this->boss->Initialize();
}
/// <summary>
/// �X�V
/// </summary>
void EnemyManager::Update()
{
	this->boss->Update();
}
/// <summary>
/// �A�N�V����
/// </summary>
void EnemyManager::Action()
{
	this->boss->Action();
}
/// <summary>
/// �`��
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss->Draw();
}

/// <summary>
/// ���W�̎擾
/// </summary>
const VECTOR EnemyManager::GetPosition()const
{
	return this->boss->GetPosition();
}


const Collider EnemyManager::GetCharacterCollider()
{
	return this->boss->GetCharacterCollider();
}
const Collider EnemyManager::GetAttackCollider()
{
	return this->boss->GetAttackCollider();
}
/// <summary>
/// �_���[�W�̎擾
/// </summary>
const int EnemyManager::GetDamage()const
{
	return this->boss->GetDamage();
}
/// <summary>
/// �_���[�W����
/// </summary>
void EnemyManager::CalcDamage(const int _damage)
{
	return this->boss->CalcDamage(_damage);
}
const int EnemyManager::GetHP()const
{
	return this->boss->GetHP();
}
const int EnemyManager::GetAttackNumber()const
{
	return this->boss->GetAttackNumber();
}
const int EnemyManager::GetHitNumber()const
{
	return this->boss->GetHitNumber();
}
void EnemyManager::SetHitNumber(const int _attackNumber)
{
	this->boss->SetHitNumber(_attackNumber);
}
const bool EnemyManager::IsAttack()const
{
	return this->boss->IsAttack();
}
void EnemyManager::OnIsHitAttack()
{
	this->boss->OnIsHitAttack();
}