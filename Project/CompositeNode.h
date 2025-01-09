//=====================================================
// @brief �R���|�W�b�g�m�[�h
// �����̎q�m�[�h���O���[�v�����A�����̓�����Ǘ�����
//=====================================================
#pragma once

class BehaviorTreeNode;
class CompositeNode abstract : public BehaviorTreeNode
{
public:
			 CompositeNode();//�R���X�g���N�^
	virtual ~CompositeNode();//�f�X�g���N�^

	virtual NodeState Update() abstract;//�X�V
	void AddChild(BehaviorTreeNode& _child) override;//�q���̒ǉ�
protected:
	std::vector<BehaviorTreeNode*> children;
};

