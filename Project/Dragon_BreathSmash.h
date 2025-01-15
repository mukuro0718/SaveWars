//=====================================================
// @brief �u���X�{�@�����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmash : public ActionNode
{
public:
	 Dragon_BreathSmash();//�R���X�g���N�^
	~Dragon_BreathSmash();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		BREATH	= 0,//�u���X
		WALK	= 1,//����
		SMASH	= 2,//�@����
	};

	/*�ÓI�萔*/
	const short MAX_USE_ANIMATION;//�g�p����A�j���[�V�����̍ő�
	const float NEAR_ATTACK_RANGE;//�ߐڍU���͈�
	const float	SWEEP_CANCEL_PLAY_TIME;//�Ȃ������U���L�����Z���t���[��

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	float			totalPlayTime;		//���Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
};

