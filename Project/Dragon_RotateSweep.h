//=====================================================
// @brief ��]�U���{�Ȃ������A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_RotateSweep : public ActionNode
{
public:
	 Dragon_RotateSweep();//�R���X�g���N�^
	~Dragon_RotateSweep();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK   = 0,//����
		ROTATE = 1,//��]�U��
		SWEEP  = 2,//�Ȃ�����
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
};

