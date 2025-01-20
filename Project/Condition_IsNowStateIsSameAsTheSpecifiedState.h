//===========================================================
// @brief �{�X�̏�Ԃ��]����������ԂƓ�����
//===========================================================
#pragma once

class ConditionNode;
class BeastBehaviorTree;
class Condition_IsNowStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsNowStateIsSameAsTheSpecifiedState(const BeastBehaviorTree::BeastState _specifiedState);//�R���X�g���N�^
	~Condition_IsNowStateIsSameAsTheSpecifiedState();//�f�X�g���N�^

	NodeState Update() override;//�X�V
private:
	const BeastBehaviorTree::BeastState SPECIFIED_STATE;
};

