//=====================================================
// @brief �E���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_TurnRight : public ActionNode
{
public:
	 Dragon_TurnRight();//�R���X�g���N�^
	~Dragon_TurnRight();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

