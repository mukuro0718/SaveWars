//===========================================================
// @brief �ڕW�Ƃ̋������萔�ȏゾ������
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonToTargetDistanceGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsDragonToTargetDistanceGreaterThanConstant(const float _compareDistance);//�R���X�g���N�^
	~Condition_IsDragonToTargetDistanceGreaterThanConstant();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const float COMPARE_DISTANCE;//��r�������萔����
};

