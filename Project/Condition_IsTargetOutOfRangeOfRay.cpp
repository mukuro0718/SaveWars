#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfRay::Condition_IsTargetOutOfRangeOfRay(const float _maxToleranceRange, const float _minToleranceRange)
	: MAX_TOLERANCE_RANGE(_maxToleranceRange)
	, MIN_TOLERANCE_RANGE(_minToleranceRange)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfRay::~Condition_IsTargetOutOfRangeOfRay()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsTargetOutOfRangeOfRay::NodeState Condition_IsTargetOutOfRangeOfRay::Update()
{
	/*�ڕW�Ƃ̋������萔�ȏゾ������*/
	auto& root = Singleton<BeastBehaviorTree>::GetInstance();
	float dot = root.GetDotOfDirAndToTarget();
	if (this->MIN_TOLERANCE_RANGE <= dot && dot <= this->MAX_TOLERANCE_RANGE)
	{
		printfDx("CONDITION_DOT_FAILURE\n");
		return NodeState::FAILURE;
	}
	printfDx("CONDITION_DOT_SUCCESS\n");
	return NodeState::SUCCESS;
}
