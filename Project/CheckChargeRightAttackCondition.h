//===========================================================
// @brief �E�����ߍU��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckChargeRightAttackCondition : public ConditionNode
{
public:
	 CheckChargeRightAttackCondition();//�R���X�g���N�^
	~CheckChargeRightAttackCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

