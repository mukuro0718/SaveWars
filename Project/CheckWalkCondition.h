//===========================================================
// @brief �������̔�����s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckWalkCondition : public ConditionNode
{
public:
	 CheckWalkCondition();//�R���X�g���N�^
	~CheckWalkCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

