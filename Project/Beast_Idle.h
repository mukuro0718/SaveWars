//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Idle : public ActionNode
{
public:
	 Beast_Idle();//�R���X�g���N�^
	~Beast_Idle();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

