#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CompositeNode::CompositeNode()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CompositeNode::~CompositeNode()
{
	this->children.clear();
}

/// <summary>
/// �q���̒ǉ�
/// </summary>
void CompositeNode::AddChild(BehaviorTreeNode& _child)
{
	this->children.emplace_back(&_child);
}