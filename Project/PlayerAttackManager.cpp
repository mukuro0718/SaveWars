#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
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
void PlayerAttackManager::Initialize(GoriLib::Physics* _physics)
{
	this->attack->Initialize(_physics);
}
/// <summary>
/// �㏈��
/// </summary>
void PlayerAttackManager::Finalize(GoriLib::Physics* _physics)
{
	this->attack->Finalize(_physics);
}
/// <summary>
/// �X�V
/// </summary>
void PlayerAttackManager::Update(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR position = player.GetPosition();
	VECTOR direction = player.GetDirection();

	/*�U�����Ă��邩�̔���*/
	if (!this->attack->GetIsStart() && player.GetIsSlash())
	{
		this->attack->OnIsStart();
	}

	this->attack->Update(_physics, position, direction);
}
/// <summary>
/// �Փ˂�����
/// </summary>
void PlayerAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->attack->OnCollide(_colider);
}

/// <summary>
/// �`��
/// </summary>
const void PlayerAttackManager::Draw()const
{
	this->attack->Draw();
}

void PlayerAttackManager::OnIsStart(const int _index)
{
	this->attack->OnIsStart();
}
