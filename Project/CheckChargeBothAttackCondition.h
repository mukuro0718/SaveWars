//===========================================================
// @brief �������ߍU��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckChargeBothAttackCondition : public ConditionNode
{
public:
	 CheckChargeBothAttackCondition();//�R���X�g���N�^
	~CheckChargeBothAttackCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

