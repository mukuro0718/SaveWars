//=====================================================
// @brief �E���U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_RightFootAttack : public ActionNode
{
public:
	 Beast_RightFootAttack();//�R���X�g���N�^
	~Beast_RightFootAttack();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

