#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
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
void PlayerManager::Initialize()
{
	this->player->Initialize();
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerManager::Finalize()
{
	this->player->Finalize();
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
/// �_���[�W�f�[�^�̎擾
/// </summary>
const CharacterData& PlayerManager::GetCharacterData()const
{
	return this->player->GetCharacterData();
}

/// <summary>
/// ���W�b�h�{�f�B�̎擾
/// </summary>
const Rigidbody& PlayerManager::GetRigidbody()const
{
	return this->player->GetRigidbody();
}

/// <summary>
/// �ړ��t���O�̎擾
/// </summary>
const bool PlayerManager::IsMove()const
{
	auto player = dynamic_cast<Player&>(*this->player);
	return player.IsMove();
}
/// <summary>
/// �V���b�g�t���O
/// </summary>
const bool PlayerManager::GetIsAttack()const
{
	return this->player->GetIsAttack();
}

const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const float PlayerManager::GetStamina()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetStamina();
}
