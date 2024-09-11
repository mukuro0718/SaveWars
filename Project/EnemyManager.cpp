#include <DxLib.h>
#include "UseSTL.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
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
void EnemyManager::Finalize()
{
	this->boss->Finalize();
}

/// <summary>
/// �`��
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss->Draw();
}

/// <summary>
/// �_���[�W�f�[�^�̎擾
/// </summary>
const CharacterData& EnemyManager::GetCharacterData()const
{
	return this->boss->GetCharacterData();
}

/// <summary>
/// ���W�b�h�{�f�B�̎擾
/// </summary>
const Rigidbody& EnemyManager::GetRigidbody()const
{
	return this->boss->GetRigidbody();
}

/// <summary>
/// �_���[�W����
/// </summary>
const int EnemyManager::GetHP()const
{
	return this->boss->GetHP();
}

/// <summary>
/// �U���t���O�̎擾
/// </summary>
const bool EnemyManager::IsAttack()const
{
	return this->boss->GetIsAttack();
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int EnemyManager::GetModelHandle()const
{
	return this->boss->GetModelHandle();
}