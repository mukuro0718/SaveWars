#include <DxLib.h>
#include "UseSTL.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "VECTORtoUseful.h"
#include "Player.h"
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
void PlayerManager::Initialize(GoriLib::Physics* _physics)
{
	this->player->Initialize(_physics);
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerManager::Finalize(GoriLib::Physics* _physics)
{
	this->player->Finalize(_physics);
}

/// <summary>
/// �X�V
/// </summary>
void PlayerManager::Update(GoriLib::Physics* _physics)
{
	this->player->Update(_physics);
}
/// <summary>
/// �`��
/// </summary>
const void PlayerManager::Draw()const
{
	this->player->Draw();
}

/// <summary>
/// ����������
/// </summary>
void PlayerManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->player->OnCollide(_colider);
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
/// �V���b�g�t���O
/// </summary>
const bool PlayerManager::GetIsSlash()const
{
	return this->player->GetIsSlash();
}

const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const float PlayerManager::GetStamina()const
{
	return this->player->GetStamina();
}
const int PlayerManager::GetHitNumber()const
{
	return this->player->GetHitNumber();
}
