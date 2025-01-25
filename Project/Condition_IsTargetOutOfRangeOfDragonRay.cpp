#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::Condition_IsTargetOutOfRangeOfDragonRay(const float _maxToleranceRange, const float _minToleranceRange)
	: MAX_TOLERANCE_RANGE(_maxToleranceRange)
	, MIN_TOLERANCE_RANGE(_minToleranceRange)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::~Condition_IsTargetOutOfRangeOfDragonRay()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::NodeState Condition_IsTargetOutOfRangeOfDragonRay::Update()
{
	/*�ڕW�Ƃ̋������萔�ȏゾ������*/
	auto& root = Singleton<DragonBehaviorTree>::GetInstance();
	float dot = root.GetDotOfDirAndToTarget();
	if (this->MIN_TOLERANCE_RANGE <= dot && dot <= this->MAX_TOLERANCE_RANGE)
	{
		//printfDx("CONDITION_DOT_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_DOT_SUCCESS\n");
	return NodeState::SUCCESS;
}
