#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonHpIsLessThanConstant::Condition_IsDragonHpIsLessThanConstant(const int _compareHp)
	: COMPARE_HP(_compareHp)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonHpIsLessThanConstant::~Condition_IsDragonHpIsLessThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonHpIsLessThanConstant::NodeState Condition_IsDragonHpIsLessThanConstant::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HP���萔�ȉ��Ȃ�SUCCESS��Ԃ�*/
	if (enemy.GetHP() <= this->COMPARE_HP)
	{
		//printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}