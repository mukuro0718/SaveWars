//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Dying : public ActionNode
{
public:
	 Beast_Dying();//�R���X�g���N�^
	~Beast_Dying();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	bool  isInitialize;		//������������
};

