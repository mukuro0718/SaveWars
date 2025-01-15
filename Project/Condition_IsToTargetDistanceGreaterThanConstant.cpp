#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::Condition_IsToTargetDistanceGreaterThanConstant(const float _compareDistance)
	: COMPARE_DISTANCE(_compareDistance)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::~Condition_IsToTargetDistanceGreaterThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::NodeState Condition_IsToTargetDistanceGreaterThanConstant::Update()
{
	/*�ڕW�Ƃ̋������萔�ȏゾ������*/
	auto& root = Singleton<BeastBehaviorTree>::GetInstance();
	if (root.GetToTargetDistance() >= this->COMPARE_DISTANCE)
	{
		//printfDx("CONDITION_DISTANCE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DISTANCE_FAILURE\n");
	return NodeState::FAILURE;
}
