//===========================================================
// @brief ���x�����グ����K�̔�����s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class CheckRaiseLevelCondition : public ConditionNode
{
public:
	 CheckRaiseLevelCondition();//�R���X�g���N�^
	~CheckRaiseLevelCondition();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
};

