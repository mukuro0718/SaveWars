#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossSlash2Attack.h"
#include "BossStabAttack.h"
#include "BossRotateSlashAttack.h"
#include "BossPunchAttack.h"
#include "BossJumpAttack.h"
#include "BossSlashComboAttack.h"
#include "BossSlashCombo2Attack.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAttackManager::BossAttackManager()
{
	this->attack.emplace_back(new BossSlashAttack		(static_cast<int>(BossAttack::AttackType::SLASH_1)));
	this->attack.emplace_back(new BossSlash2Attack		(static_cast<int>(BossAttack::AttackType::SLASH_2)));
	this->attack.emplace_back(new BossStabAttack		(static_cast<int>(BossAttack::AttackType::STAB)));
	this->attack.emplace_back(new BossRotateSlashAttack	(static_cast<int>(BossAttack::AttackType::ROTATE_SLASH)));
	this->attack.emplace_back(new BossPunchAttack		(static_cast<int>(BossAttack::AttackType::PUNCH)));
	this->attack.emplace_back(new BossSlashComboAttack	(static_cast<int>(BossAttack::AttackType::SLASH_COMBO_1)));
	this->attack.emplace_back(new BossSlashCombo2Attack	(static_cast<int>(BossAttack::AttackType::SLASH_COMBO_2)));
	this->attack.emplace_back(new BossJumpAttack		(static_cast<int>(BossAttack::AttackType::JUMP_ATTACK)));
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
