#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::Condition_IsDragonToTargetDistanceGreaterThanConstant(const float _compareDistance)
	: COMPARE_DISTANCE(_compareDistance)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::~Condition_IsDragonToTargetDistanceGreaterThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::NodeState Condition_IsDragonToTargetDistanceGreaterThanConstant::Update()
{
	/*�ڕW�Ƃ̋������萔�ȏゾ������*/
	auto& root = Singleton<DragonBehaviorTree>::GetInstance();
	if (root.GetToTargetDistance() >= this->COMPARE_DISTANCE)
	{
		//printfDx("CONDITION_DISTANCE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DISTANCE_FAILURE\n");
	return NodeState::FAILURE;
}
