//=============================================
// @brief�@�X���b�V���R���{�A�N�V����Ver.1
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashComboAction : public BossAction
{
public:
	 BossSlashComboAction();//�R���X�g���N�^
	~BossSlashComboAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool	isClose;			//��x�ł��߂Â�����
	float	hitStopTime;		//�q�b�g�X�g�b�v����
	int		hitStopDelay;		//�q�b�g�X�g�b�v�f�B���C
	int		hitStopType;		//�q�b�g�X�g�b�v�̎��
	float	slowFactor;			//�X���[�W��
	VECTOR	rotateLerpValue;	//��]�␳��
	short	rotateFixFrame;		//��]�␳�p
	short	moveFrame;			//�ړ��p
	short	rotateFrame;		//��]�p
	float	stopDistance;		//��~���鋗��
	float	speed;				//���x
	short	maxInterval;		//�ő�C���^�[�o��
	short	checkState;			//�ǂ̏�ԂŊm�F���邩
	float	actionDistance;		//�A�N�V�������\�ɂȂ鋗��
};

