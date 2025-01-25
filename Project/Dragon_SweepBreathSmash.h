//=====================================================
// @brief �Ȃ������{�u���X�{�@�����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepBreathSmash : public ActionNode
{
public:
	 Dragon_SweepBreathSmash();//�R���X�g���N�^
	~Dragon_SweepBreathSmash();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK_1	= 0,//����
		SWEEP	= 1,//�Ȃ�����
		BREATH	= 2,//�u���X
		WALK_2	= 3,//����
		SMASH	= 4,//�@����
	};

	/*�萔*/
	const int FIX_ROTATE_FRAME;//��]�␳�t���[��

	/*�����o�ϐ�*/
	vector<short>	 animationType;		 //�A�j���[�V�����̎��
	vector<float>	 animationPlayTime;	 //�A�j���[�V�����Đ�����
	short			 useAnimationType;	 //�g�p����A�j���[�V�����̃^�C�v
	short			 maxUseAnimation;	 //�g�p����A�j���[�V�����̍ő�
	float			 nearAttackRange;	 //�ߐڍU���͈�
	map<short, short>useColliderIndex;	 //�g�p����R���C�_�[
	float			 nowTotalPlayTime;	 //���݂̑��Đ�����
	bool			 isClose;			 //�߂Â�����
	short			 frameCount;		 //�t���[���J�E���g
};

