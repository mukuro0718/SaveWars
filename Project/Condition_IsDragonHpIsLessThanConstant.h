//===========================================================
// @brief HP���萔�ȉ���
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonHpIsLessThanConstant : public ConditionNode
{
public:
	 Condition_IsDragonHpIsLessThanConstant(const int _compareHp);//�R���X�g���N�^
	~Condition_IsDragonHpIsLessThanConstant();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int COMPARE_HP;//��r�������̗�
};

