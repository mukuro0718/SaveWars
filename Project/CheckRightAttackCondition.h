//===========================================================
// @brief �E���U��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckRightAttackCondition : public ConditionNode
{
public:
	 CheckRightAttackCondition();//�R���X�g���N�^
	~CheckRightAttackCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

