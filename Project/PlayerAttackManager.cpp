#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "PlayerAttack.h"
#include "PlayerAttackManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerAttackManager::PlayerAttackManager()
{
	this->attack = new PlayerAttack();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerAttackManager::~PlayerAttackManager()
{
	DeleteMemberInstance(this->attack);
}

/// <summary>
/// ������
/// </summary>
void PlayerAttackManager::Initialize()
{
	this->attack->Initialize();
}
/// <summary>
/// �㏈��
/// </summary>
void PlayerAttackManager::Finalize()
{
	this->attack->Finalize();
}
/// <summary>
/// �X�V
/// </summary>
void PlayerAttackManager::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR position = player.GetRigidbody().GetPosition();
	VECTOR direction = player.GetRigidbody().GetDirection();

	this->attack->Update(position, direction);
}

/// <summary>
/// �`��
/// </summary>
const void PlayerAttackManager::Draw()const
{
	this->attack->Draw();
}

void PlayerAttackManager::OnIsStart()
{
	this->attack->OnIsStart();
}

/// <summary>
/// �_���[�W�̃Z�b�g
/// </summary>
void PlayerAttackManager::SetDamage(const float _damage)
{
	this->attack->SetDamage(_damage);
}
