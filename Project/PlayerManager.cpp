#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
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
/// HP�̎擾
/// </summary>
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}

/// <summary>
/// ���݂̏�Ԃ��擾
/// </summary>
const int PlayerManager::GetNowState()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetNowState();
}

/// <summary>
/// �X�^�~�i�̎擾
/// </summary>
const float PlayerManager::GetStamina()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetStamina();
}

/// <summary>
/// �񕜃I�[�u�̐��̎擾
/// </summary>
const int PlayerManager::GetHealOrbNum()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetHealCount();
}

/// <summary>
/// �����t���O��getter
/// </summary>
const bool PlayerManager::GetIsAlive()const
{
	return this->player->GetIsAlive();
}

/// <summary>
/// ���b�N�I���t���O�̎擾
/// </summary>
const bool PlayerManager::GetIsLockOn()const
{
	return false;
}

/// <summary>
/// �����t���O�̎擾
/// </summary>
const bool PlayerManager::GetIsDrawSword()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetIsDrawSword();
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int PlayerManager::GetModelHandle() const
{
	return this->player->GetModelHandle();
}