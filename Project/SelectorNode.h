//=====================================================
// @brief �Z���N�^�[�m�[�h
// �q�����Ԃɕ]�����A�ŏ��ɐ��������q�m�[�h�����s����
// ���ׂĂ̎q�����s�����ꍇ�A���s�Ƃ��Č��ʂ�Ԃ�
//=====================================================
#pragma once

class CompositeNode;
class SelectorNode : public CompositeNode
{
public:
	 SelectorNode();//�R���X�g���N�^
	~SelectorNode();//�f�X�g���N�^

	NodeState Update() override;//�X�V
};

