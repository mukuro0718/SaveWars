//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Idle : public ActionNode
{
public:
	 Dragon_Idle();//�R���X�g���N�^
	~Dragon_Idle();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�萔*/
	const short AWAKENING_STANDBY_TIME;	//�ҋ@����
	const short FURY_STANDBY_TIME;		//�ҋ@����
	const short RAMPAGE_STANDBY_TIME;	//�ҋ@����

	/*�����o�ϐ�*/
	short currentStandbyTime;	//���݂̑ҋ@����
	short standbyTime;			//�ҋ@����
};

