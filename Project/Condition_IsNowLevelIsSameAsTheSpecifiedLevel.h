//===========================================================
// @brief �E�����ߍU��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsNowLevelIsSameAsTheSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel);//�R���X�g���N�^
	~Condition_IsNowLevelIsSameAsTheSpecifiedLevel();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int SPECIFIED_LEVEL;
};

