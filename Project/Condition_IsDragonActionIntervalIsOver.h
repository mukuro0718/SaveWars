//========================================================================
// @brief �A�N�V�����̃C���^�[�o�����I�����Ă��邩���m�F����R���f�B�V�����m�[�h
//========================================================================
#pragma once

class ConditionNode;
class Condition_IsDragonActionIntervalIsOver : public ConditionNode
{
public:
	 Condition_IsDragonActionIntervalIsOver(const int _actionIndex);//�R���X�g���N�^
	~Condition_IsDragonActionIntervalIsOver();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int ACTION_INDEX;//�A�N�V�����ԍ�
};

