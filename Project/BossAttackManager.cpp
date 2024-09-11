#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossHurricaneKick.h"
#include "BossJumpAttack.h"
#include "BossFlyAttack.h"
#include "BossRotatePunch.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAttackManager::BossAttackManager()
{
	this->attack.emplace_back(new BossSlashAttack	(static_cast<int>(AttackType::SLASH)));
	this->attack.emplace_back(new BossFlyAttack		(static_cast<int>(AttackType::FLY_ATTACK)));
	this->attack.emplace_back(new BossHurricaneKick	(static_cast<int>(AttackType::HURRICANE_KICK)));
	this->attack.emplace_back(new BossJumpAttack	(static_cast<int>(AttackType::JUMP_ATTACK)));
	this->attack.emplace_back(new BossRotatePunch	(static_cast<int>(AttackType::ROTATE_PUNCH)));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAttackManager::~BossAttackManager()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		DeleteMemberInstance(this->attack[i]);
	}
}

/// <summary>
/// ������
/// </summary>
void BossAttackManager::Initialize()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Initialize();
	}
}
/// <summary>
/// �㏈��
/// </summary>
void BossAttackManager::Finalize()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Finalize();
	}
}
/// <summary>
/// �X�V
/// </summary>
void BossAttackManager::Update()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Update();
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossAttackManager::Draw()const
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Draw();
	}
}

void BossAttackManager::OnIsStart(const int _index)
{
	this->attack[_index]->OnIsStart();
}
const VECTOR BossAttackManager::GetPosition(const int _index)
{
	return this->attack[_index]->GetPosition();
}
