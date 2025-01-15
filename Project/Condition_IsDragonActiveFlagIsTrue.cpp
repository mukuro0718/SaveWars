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
Condition_IsDragonActiveFlagIsTrue::Condition_IsDragonActiveFlagIsTrue()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonActiveFlagIsTrue::~Condition_IsDragonActiveFlagIsTrue()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonActiveFlagIsTrue::NodeState Condition_IsDragonActiveFlagIsTrue::Update()
{
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();

	/*�X�^�~�i���O��葽�����SUCCESS��Ԃ�*/
	if (rootNode.GetIsActive())
	{
		//printfDx("CONDITION_ACTIVE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_ACTIVE_FAILURE\n");
	return NodeState::FAILURE;
}