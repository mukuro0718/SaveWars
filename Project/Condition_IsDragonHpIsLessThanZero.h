//===========================================================
// @brief HP���O�ȉ���
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonHpIsLessThanZero : public ConditionNode
{
public:
	 Condition_IsDragonHpIsLessThanZero();//�R���X�g���N�^
	~Condition_IsDragonHpIsLessThanZero();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

