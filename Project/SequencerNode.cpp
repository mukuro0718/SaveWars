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
	for (auto& child : this->children)
	{
		if (child->Update() == NodeState::FAILURE)
		{
			return NodeState::FAILURE;
		}
	}
	return NodeState::SUCCESS;
}