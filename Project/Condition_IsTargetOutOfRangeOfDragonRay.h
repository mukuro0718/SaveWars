//===========================================================
// @brief �ڕW��Ray�̊O��������
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsTargetOutOfRangeOfDragonRay : public ConditionNode
{
public:
	 Condition_IsTargetOutOfRangeOfDragonRay(const float _maxToleranceRange, const float _minToleranceRange);//�R���X�g���N�^
	~Condition_IsTargetOutOfRangeOfDragonRay();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const float MAX_TOLERANCE_RANGE;//�ŏ����e�͈�
	const float MIN_TOLERANCE_RANGE;//�ő勖�e�͈�
};

