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


const Collider EnemyManager::GetCollider()
{
	return this->boss->GetCollider();
}
