#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "SequencerNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SequencerNode::SequencerNode()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SequencerNode::~SequencerNode()
{

}

/// <summary>
/// �X�V
/// </summary>
SequencerNode::NodeState SequencerNode::Update()
{
	/*�q�����s*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		//���s���Ă������Ԃ�Ԃ�
		if (state == NodeState::FAILURE) return NodeState::FAILURE;
		//���s���Ȃ��Ԃ�Ԃ�
		if (state == NodeState::RUNNING) return NodeState::RUNNING;
	}
	return NodeState::SUCCESS;
}