//=====================================================
// @brief �_�E���A�N�V����
// ���݂͎������Ă��Ȃ�
// �K�v�Ȃ��������
//=====================================================
#pragma once

class ActionNode;
class Beast_Down : public ActionNode
{
public:
	 Beast_Down();//�R���X�g���N�^
	~Beast_Down();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	bool isInitialize;//������������
};

