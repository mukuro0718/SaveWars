//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Walk : public ActionNode
{
public:
	 Dragon_Walk();//�R���X�g���N�^
	~Dragon_Walk();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

