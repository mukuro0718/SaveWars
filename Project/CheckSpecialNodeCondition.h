//===========================================================
// @brief ����U�����s���邩�̔�����s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckSpecialNodeCondition : public ConditionNode
{
public:
	 CheckSpecialNodeCondition();//�R���X�g���N�^
	~CheckSpecialNodeCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

