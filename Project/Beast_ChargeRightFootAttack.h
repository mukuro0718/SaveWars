//=====================================================
// @brief �E�����ߍU���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_ChargeRightFootAttack : public ActionNode
{
public:
	Beast_ChargeRightFootAttack();//�R���X�g���N�^
	~Beast_ChargeRightFootAttack();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

