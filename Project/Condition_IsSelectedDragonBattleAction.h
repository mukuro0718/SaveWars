//===========================================================
// @brief �o�g���A�N�V�������I������Ă��邩
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedDragonBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedDragonBattleAction();//�R���X�g���N�^
	~Condition_IsSelectedDragonBattleAction();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

