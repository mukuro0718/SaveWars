//===========================================================
// @brief NORMAL��ANGRY�ւ̏�ԕω����m�F����R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckAngryCondition : public ConditionNode
{
public:
	 CheckAngryCondition();//�R���X�g���N�^
	~CheckAngryCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

