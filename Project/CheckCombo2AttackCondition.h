//===========================================================
// @brief �R���{�Q�U��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckCombo2AttackCondition : public ConditionNode
{
public:
	 CheckCombo2AttackCondition();//�R���X�g���N�^
	~CheckCombo2AttackCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

