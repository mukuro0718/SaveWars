//===========================================
// @brief �A�N�V�����m�[�h
// �Q�[���L�����N�^�[�̎��ۂ̂ӂ�܂�������
//===========================================
#pragma once

class BehaviorTreeNode;
class ActionNode abstract : public BehaviorTreeNode
{
public:
	/*�񋓑�*/


	ActionNode(){}//�R���X�g���N�^
	virtual ~ActionNode(){}//�f�X�g���N�^

	virtual NodeState Update() abstract;//�X�V
};

