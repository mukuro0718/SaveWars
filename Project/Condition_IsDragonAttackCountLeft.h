//===========================================================
// @brief �c��U���񐔂��O��
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonAttackCountLeft : public ConditionNode
{
public:
	 Condition_IsDragonAttackCountLeft();//�R���X�g���N�^
	~Condition_IsDragonAttackCountLeft();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

