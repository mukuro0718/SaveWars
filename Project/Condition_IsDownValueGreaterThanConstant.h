//=======================================================
// @brief �_�E�����m�F����R���f�B�V�����m�[�h
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsDownValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsDownValueGreaterThanConstant(const int _specifiedDownValue);//�R���X�g���N�^
	~Condition_IsDownValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update() override;//�X�V

private:
	const int SPECIFIED_DOWN_VALUE;//�_�E���l

};

