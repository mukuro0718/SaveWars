//=====================================================
// @brief ���ɉ�����Ȃ�����K�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_BackingBreath : public ActionNode
{
public:
	 Beast_BackingBreath();//�R���X�g���N�^
	~Beast_BackingBreath();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

