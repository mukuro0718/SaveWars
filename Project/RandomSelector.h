//=====================================================
// @brief �����_���Z���N�^�[�m�[�h
// �q�m�[�h�������_���őI������
//=====================================================
#pragma once

class CompositeNode;
class RandomSelector : public CompositeNode
{
public:
			 RandomSelector();//�R���X�g���N�^
	virtual ~RandomSelector();//�f�X�g���N�^

	NodeState Update() override;//�X�V
};

