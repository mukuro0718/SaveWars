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
Condition_IsStageIsDifferent::Condition_IsStageIsDifferent()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsStageIsDifferent::~Condition_IsStageIsDifferent()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsStageIsDifferent::NodeState Condition_IsStageIsDifferent::Update()
{
	/*���ʔj�󂳂�Ă����琬��*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.JudgeBossStage())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}