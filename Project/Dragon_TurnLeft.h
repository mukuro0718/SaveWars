//=====================================================
// @brief ������A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_TurnLeft : public ActionNode
{
public:
	 Dragon_TurnLeft();//�R���X�g���N�^
	~Dragon_TurnLeft();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

