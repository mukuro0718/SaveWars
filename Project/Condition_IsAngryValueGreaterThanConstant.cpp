#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsAngryValueGreaterThanConstant::Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue)
	: SPECIFIED_ANGRY_VALUE(_specifiedAngryValue)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsAngryValueGreaterThanConstant::~Condition_IsAngryValueGreaterThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsAngryValueGreaterThanConstant::NodeState Condition_IsAngryValueGreaterThanConstant::Update()
{
	/*怒り値が０以上なら成功を返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetAngryValue() >= this->SPECIFIED_ANGRY_VALUE)
	{
		//printfDx("CONDITION_ANGRY_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_ANGRY_FAILURE\n");
	return NodeState::FAILURE;
}
