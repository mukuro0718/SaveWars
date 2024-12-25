//=============================================
// @brief �{�X�W�����v�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossJumpAttackAction : public BossAction
{
public:
	 BossJumpAttackAction();//�R���X�g���N�^
	~BossJumpAttackAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool	isClose;			//�߂Â�����
	float	hitStopTime;		//�q�b�g�X�g�b�v����
	int		hitStopDelay;		//�q�b�g�X�g�b�v�f�B���C
	int		hitStopType;		//�q�b�g�X�g�b�v�̎��
	float	slowFactor;			//�X���[�W��
	VECTOR	rotateLerpValue;	//��]�␳��
	float	targetOffset;		//�ڕW�␳
	short	rotateFixFrame;		//��]�␳�p
	short	moveFrame;			//�ړ��p
	short	rotateFrame;		//��]�p
	float	stopDistance;		//��~���鋗��
	float	speed;				//���x
	short	maxInterval;		//�ő�C���^�[�o��
	short	checkState;			//�ǂ̏�ԂŊm�F���邩
	float	actionDistance;		//�A�N�V�������\�ɂȂ鋗��
};

