//===========================================================
// @brief �A�N�e�B�u��Ԃ����肷��R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonActiveFlagIsTrue : public ConditionNode
{
public:
	 Condition_IsDragonActiveFlagIsTrue();//�R���X�g���N�^
	~Condition_IsDragonActiveFlagIsTrue();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

