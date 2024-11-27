#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "Beast.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyManager::EnemyManager()
{
	this->boss.emplace_back(new Boss());
	this->boss.emplace_back(new Beast());
	this->boss.emplace_back(new Dragon());
	this->boss.emplace_back(new Boss());
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
	auto& changer = Singleton<EnemyChanger>::GetInstance();
	this->enemyType = changer.GetEnemyType();
	this->boss[this->enemyType]->Initialize();
}
/// <summary>
/// �X�V
/// </summary>
void EnemyManager::Update()
{
	this->boss[this->enemyType]->Update();
}
/// <summary>
/// �A�N�V����
/// </summary>
void EnemyManager::Finalize()
{
	this->boss[this->enemyType]->Finalize();
}

/// <summary>
/// �`��
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss[this->enemyType]->Draw();
}

/// <summary>
/// �_���[�W�f�[�^�̎擾
/// </summary>
const CharacterData& EnemyManager::GetCharacterData()const
{
	return this->boss[this->enemyType]->GetCharacterData();
}

/// <summary>
/// ���W�b�h�{�f�B�̎擾
/// </summary>
const Rigidbody& EnemyManager::GetRigidbody()const
{
	return this->boss[this->enemyType]->GetRigidbody();
}

/// <summary>
/// �_���[�W����
/// </summary>
const int EnemyManager::GetHP()const
{
	return this->boss[this->enemyType]->GetHP();
}

/// <summary>
/// �U���t���O�̎擾
/// </summary>
const bool EnemyManager::IsAttack()const
{
	return this->boss[this->enemyType]->GetIsAttack();
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int EnemyManager::GetModelHandle()const
{
	return this->boss[this->enemyType]->GetModelHandle();
}

const bool EnemyManager::GetIsAlive()const
{
	return this->boss[this->enemyType]->GetIsAlive();
}