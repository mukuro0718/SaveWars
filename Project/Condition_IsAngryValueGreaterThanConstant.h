//=======================================================
// @brief �{��l���m�F����R���f�B�V�����m�[�h
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsAngryValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue);//�R���X�g���N�^
	~Condition_IsAngryValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update() override;//�X�V

private:
	const int SPECIFIED_ANGRY_VALUE;//�{��l

};

