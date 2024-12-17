#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "SelectorNode.h"

SelectorNode::SelectorNode()
{

}


SelectorNode::~SelectorNode()
{

}

/// <summary>
/// �X�V
/// </summary>
SelectorNode::NodeState SelectorNode::Update()
{
	/*�q�����s*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		//��Ԃ������Ȃ琬����Ԃ��I��
		if (state == NodeState::SUCCESS) return NodeState::SUCCESS;
		//��Ԃ����s���Ȃ���s����Ԃ��I��
		if (state == NodeState::RUNNING) return NodeState::RUNNING;
	}
	return NodeState::FAILURE;
}