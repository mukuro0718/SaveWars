//=======================================================
// @brief HP���m�F����R���f�B�V�����m�[�h
//=======================================================
#pragma once

class ConditionNode;
class CheckDyingCondition : public ConditionNode
{
public:
	 CheckDyingCondition();//�R���X�g���N�^
	~CheckDyingCondition();//�f�X�g���N�^

	NodeState Update() override;//�X�V

private:
	/*�����o�ϐ�*/
	
};

