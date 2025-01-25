#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsStageIsDifferent::Condition_IsStageIsDifferent()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsStageIsDifferent::~Condition_IsStageIsDifferent()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsStageIsDifferent::NodeState Condition_IsStageIsDifferent::Update()
{
	/*部位破壊されていたら成功*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.JudgeBossStage())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}