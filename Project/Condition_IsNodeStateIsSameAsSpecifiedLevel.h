//===========================================================
// @brief �m�[�h�̏�Ԃ��]����������ԂƓ�����
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsNodeStateIsSameAsSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNodeStateIsSameAsSpecifiedLevel(const BehaviorTreeNode::NodeState _specifiedState);//�R���X�g���N�^
	~Condition_IsNodeStateIsSameAsSpecifiedLevel();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const BehaviorTreeNode::NodeState SPECIFIED_STATE;
};

