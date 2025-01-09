//===========================================================
// @brief 目標との距離が定数以上だったら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsTargetOutOfRangeOfRay : public ConditionNode
{
public:
	Condition_IsTargetOutOfRangeOfRay(const float _maxToleranceRange, const float _minToleranceRange);//コンストラクタ
	~Condition_IsTargetOutOfRangeOfRay();//デストラクタ

	NodeState Update	() override;//更新
private:
	const float MAX_TOLERANCE_RANGE;//最小許容範囲
	const float MIN_TOLERANCE_RANGE;//最大許容範囲
};

