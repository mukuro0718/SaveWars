//=====================================================
// @brief ���K�A�N�V����
// �ʏ큨�{��ւ̏�ԕω����s���Ƃ��̙��K
//=====================================================
#pragma once

class ActionNode;
class Beast_Roar : public ActionNode
{
public:
	 Beast_Roar();//�R���X�g���N�^
	~Beast_Roar();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

