//===========================================================
// @brief �O�̃A�N�V�������A�]���������A�N�V�����Ɠ�����
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsPrevActionIsSameAsSpecifiedAction : public ConditionNode
{
public:
	 Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction);//�R���X�g���N�^
	~Condition_IsPrevActionIsSameAsSpecifiedAction();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int SPECIFIED_ACTION;
};

