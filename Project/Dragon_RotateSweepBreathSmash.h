//=====================================================
// @brief ��]�U���{�Ȃ������{�u���X�{�@�����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_RotateSweepBreathSmash : public ActionNode
{
public:
	 Dragon_RotateSweepBreathSmash();//�R���X�g���N�^
	~Dragon_RotateSweepBreathSmash();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK_1	= 0,//����
		ROTATE	= 1,//��]�U��
		SWEEP	= 2,//�Ȃ�����
		BREATH	= 3,//�u���X
		WALK_2	= 4,//����
		SMASH	= 5,//�@����
	};

	/*�萔*/
	const int FIX_ROTATE_FRAME;//��]�␳�t���[��

	/*�����o�ϐ�*/
	vector<short>	 animationType;		 //�A�j���[�V�����̎��
	vector<float>	 animationPlayTime;	 //�A�j���[�V�����Đ�����
	short			 useAnimationType;	 //�g�p����A�j���[�V�����̃^�C�v
	short			 maxUseAnimation;	 //�g�p����A�j���[�V�����̍ő�
	float			 nearAttackRange;	 //�ߐڍU���͈�
	float			 sweepCancelPlayTime;//�Ȃ������U���L�����Z���t���[��
	map<short, short>useColliderIndex;	 //�g�p����R���C�_�[
	float			 nowTotalPlayTime;	 //���݂̑��Đ�����
	bool			 isClose;			 //�߂Â�����
	short			 frameCount;		 //�t���[���J�E���g

};

