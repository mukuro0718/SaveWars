//===========================================================
// @brief �ːi�U��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckRushCondition : public ConditionNode
{
public:
	 CheckRushCondition();//�R���X�g���N�^
	~CheckRushCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

