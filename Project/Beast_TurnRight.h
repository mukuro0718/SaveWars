//=====================================================
// @brief �E�ɉ��A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_TurnRight : public ActionNode
{
public:
	 Beast_TurnRight();//�R���X�g���N�^
	~Beast_TurnRight();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

