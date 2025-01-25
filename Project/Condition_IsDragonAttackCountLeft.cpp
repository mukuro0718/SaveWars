#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonAttackCountLeft::Condition_IsDragonAttackCountLeft()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonAttackCountLeft::~Condition_IsDragonAttackCountLeft()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonAttackCountLeft::NodeState Condition_IsDragonAttackCountLeft::Update()
{
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();

	/*�U���񐔂��c���Ă���Ȃ�SUCCESS��Ԃ�*/
	if (rootNode.GetAttackCount() > 0)
	{
		//printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}