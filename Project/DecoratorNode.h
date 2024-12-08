//=====================================================
// @brief �f�R���[�^�[�m�[�h
// �q�̂ӂ�܂���ύX����m�[�h
// ��ɃA�N�V�����̎��s�̑O��Œǉ��̏������s������A
// �]�����ʂ�ύX�����肷��
//=====================================================
#pragma once

class BehaviorTreeNode;
class DecoratorNode abstract: public BehaviorTreeNode
{
public:
			 DecoratorNode();//�R���X�g���N�^
	virtual ~DecoratorNode();//�f�X�g���N�^

	virtual NodeState Update()abstract;//�X�V
	void AddChild(BehaviorTreeNode* _child) override;//�q���̒ǉ�
protected:
	BehaviorTreeNode* child;
};

