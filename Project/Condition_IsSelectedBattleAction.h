//===========================================================
// @brief �o�g���A�N�V�������I������Ă��邩
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedBattleAction();//�R���X�g���N�^
	~Condition_IsSelectedBattleAction();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

