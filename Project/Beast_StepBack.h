//=====================================================
// @brief �X�e�b�v�o�b�N�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_StepBack : public ActionNode
{
public:
	 Beast_StepBack();//�R���X�g���N�^
	~Beast_StepBack();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

