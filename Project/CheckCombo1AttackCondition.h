//===========================================================
// @brief �R���{�P�U��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckCombo1AttackCondition : public ConditionNode
{
public:
	 CheckCombo1AttackCondition();//�R���X�g���N�^
	~CheckCombo1AttackCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

