//=======================================================
// @brief ���ݒl���萔�ȏォ
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsFrighteningValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue);//�R���X�g���N�^
	~Condition_IsFrighteningValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update() override;//�X�V
private:
	const int SPECIFIED_FRIGHTENING_VALUE;//���ݒl
};

