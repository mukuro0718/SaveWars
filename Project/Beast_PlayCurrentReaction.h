//=====================================================
// @brief �ۑ������A�N�V���������s
//=====================================================
#pragma once

class ActionNode;
class Beast_PlayCurrentReaction : public ActionNode
{
public:
	 Beast_PlayCurrentReaction();//�R���X�g���N�^
	~Beast_PlayCurrentReaction();//�f�X�g���N�^

	void		Initialize	();			//������
	NodeState	Update		()override;	//�X�V
};

