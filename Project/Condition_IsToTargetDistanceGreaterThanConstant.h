//===========================================================
// @brief �ڕW�Ƃ̋������萔�ȏゾ������
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsToTargetDistanceGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsToTargetDistanceGreaterThanConstant(const float _compareDistance);//�R���X�g���N�^
	~Condition_IsToTargetDistanceGreaterThanConstant();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const float COMPARE_DISTANCE;//��r�������萔����
};

