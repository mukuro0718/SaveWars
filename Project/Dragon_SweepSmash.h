//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Idle : public ActionNode
{
public:
	 Dragon_Idle();//�R���X�g���N�^
	~Dragon_Idle();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

