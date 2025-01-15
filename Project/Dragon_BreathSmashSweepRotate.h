//=====================================================
// @brief �u���X�{�@�����{�Ȃ������{��]�U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmashSweepRotate : public ActionNode
{
public:
	 Dragon_BreathSmashSweepRotate();//�R���X�g���N�^
	~Dragon_BreathSmashSweepRotate();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		BREATH = 0,//�u���X
		WALK   = 1,//����
		SMASH  = 2,//�@����
		SWEEP  = 3,//�Ȃ�����
		ROTATE = 4,//��]�U��
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
	float			smashCancelPlayTime;//�@�����U���L�����Z���t���[��
	float			sweepCancelPlayTime;//�Ȃ������U���L�����Z���t���[��
	float			totalPlayTime;		//���Đ�����
};

