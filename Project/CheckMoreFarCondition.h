//===========================================================
// @brief �������̔�����s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckMoreFarCondition : public ConditionNode
{
public:
	 CheckMoreFarCondition();//�R���X�g���N�^
	~CheckMoreFarCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

