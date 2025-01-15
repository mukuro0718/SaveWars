//=====================================================
// @brief �@�����{��]�U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_SmashRotate : public ActionNode
{
public:
	 Dragon_SmashRotate();//�R���X�g���N�^
	~Dragon_SmashRotate();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK	= 0,//����
		SMASH	= 1,//�@����
		ROTATE	= 2,//��]�U��
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
	float			smashCancelPlayTime;//�@�����U���L�����Z���t���[��
	float			totalPlayTime;		//���Đ�����
};

