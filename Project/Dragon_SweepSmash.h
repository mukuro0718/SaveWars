//=====================================================
// @brief �Ȃ������{�@�����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepSmash : public ActionNode
{
public:
	 Dragon_SweepSmash();//�R���X�g���N�^
	~Dragon_SweepSmash();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK  = 0,//����
		SWEEP = 1,//�Ȃ�����
		SMASH = 2,//�@����
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
	float			sweepCancelPlayTime;//�Ȃ������U���L�����Z���t���[��
	float			totalPlayTime;		//���Đ�����

};

