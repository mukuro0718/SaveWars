//===========================================================
// @brief �X�[�p�[�m���@�̔�����s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckSuperNovaCondition : public ConditionNode
{
public:
	 CheckSuperNovaCondition();//�R���X�g���N�^
	~CheckSuperNovaCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

