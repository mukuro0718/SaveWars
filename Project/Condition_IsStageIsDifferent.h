//===========================================================
// @brief �h���S���̒i�K���قȂ��Ă�����
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsStageIsDifferent : public ConditionNode
{
public:
	 Condition_IsStageIsDifferent();//�R���X�g���N�^
	~Condition_IsStageIsDifferent();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

