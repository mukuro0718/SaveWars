//=====================================================
// @brief ���K�A�N�V����
// �ʏ큨�{��ւ̏�ԕω����s���Ƃ��̙��K
//=====================================================
#pragma once

class ActionNode;
class Beast_FootComboAttack : public ActionNode
{
public:
	 Beast_FootComboAttack();//�R���X�g���N�^
	~Beast_FootComboAttack();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

