#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "BossAttack.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAttackManager::BossAttackManager()
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack.emplace_back(new BossAttack(i));
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAttackManager::~BossAttackManager()
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		DeleteMemberInstance(this->attack[i]);
	}
}

/// <summary>
/// ������
/// </summary>
void BossAttackManager::Initialize(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->Initialize(_physics);
	}
}
/// <summary>
/// �㏈��
/// </summary>
void BossAttackManager::Finalize(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->Finalize(_physics);
	}
}
/// <summary>
/// �X�V
/// </summary>
void BossAttackManager::Update(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR position = enemy.GetPosition();
	VECTOR direction = VSub(player.GetPosition(), position);
	direction = VNorm(direction);

	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		if (i == static_cast<int>(AttackType::THROW_STORN))
		{
			this->attack[i]->Update(_physics, position, direction, true, 0.8f);
		}
		else
		{
			this->attack[i]->Update(_physics, position, direction, false, 0.0f);
		}
	}
}
/// <summary>
/// �Փ˂�����
/// </summary>
void BossAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->OnCollide(_colider);
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossAttackManager::Draw()const
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->Draw();
	}
}

void BossAttackManager::OnIsStart(const int _index)
{
	this->attack[_index]->OnIsStart();
}
