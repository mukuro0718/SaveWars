//=====================================================
// @brief �_�b�V���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Run : public ActionNode
{
public:
	 Beast_Run();//�R���X�g���N�^
	~Beast_Run();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

