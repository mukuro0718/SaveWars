//===========================================================
// @brief ���A�N�V�������I������Ă��邩
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedReaction : public ConditionNode
{
public:
	 Condition_IsSelectedReaction();//�R���X�g���N�^
	~Condition_IsSelectedReaction();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

