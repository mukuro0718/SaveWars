#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "ParalellNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ParalellNode::ParalellNode()
	: successCount(0)
	, failureCount(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ParalellNode::~ParalellNode()
{

}

/// <summary>
/// �X�V
/// </summary>
ParalellNode::NodeState ParalellNode::Update()
{
	/*�q�����ׂĎ��s*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		if (state == NodeState::SUCCESS)
		{
			this->successCount++;
		}
		else if (state == NodeState::FAILURE)
		{
			this->failureCount++;
		}
	}

	/*����*/
	if (this->successCount > this->successThreshold)
	{
		return NodeState::SUCCESS;
	}
	if (this->failureCount > this->failureThreshold)
	{
		return NodeState::FAILURE;
	}
	return NodeState::RUNNING;
}