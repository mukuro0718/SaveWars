//===========================================================
// @brief�ǂ����̕��ʂ��j�󂳂�Ă�����
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDestoroyedPart : public ConditionNode
{
public:
	 Condition_IsDestoroyedPart();//�R���X�g���N�^
	~Condition_IsDestoroyedPart();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

