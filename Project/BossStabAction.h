//=============================================
// @brief �{�X�˂��h���U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossStabAction : public BossAction
{
public:
	 BossStabAction();//�R���X�g���N�^
	~BossStabAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	bool	isClose;			//�߂Â�����
	float	hitStopTime;		//�q�b�g�X�g�b�v����
	int		hitStopDelay;		//�q�b�g�X�g�b�v�f�B���C
	int		hitStopType;		//�q�b�g�X�g�b�v�̎��
	float	slowFactor;			//�X���[�W��
	short	rotateFixFrame;		//��]�␳�p
	short	moveFrame;			//�ړ��p
	short	stopFrame;			//��~�p
	short	rotateFrame;		//��]�p
	short	homingFrame;		//�ǐ՗p
	float	stopDistance;		//��~���鋗��
	VECTOR	rotateLerpValue;	//��]�␳��
	float	speed;				//���x
	float	slowPlayTime;		//�X���[���̍Đ�����
	short	maxInterval;		//�ő�C���^�[�o��
	float	actionDistance;		//�A�N�V�������\�ɂȂ鋗��
	short	normalDisireValue;	//�ő�~���l
	short	checkState;			//�ǂ̏�ԂŊm�F���邩
};

