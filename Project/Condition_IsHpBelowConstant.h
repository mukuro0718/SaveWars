//===========================================================
// @brief HP���萔�ȉ���������
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsHpBelowConstant : public ConditionNode
{
public:
	 Condition_IsHpBelowConstant(const int _specifiedHp);//�R���X�g���N�^
	~Condition_IsHpBelowConstant();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int SPECIFIED_HP;//HP
};

