//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashCombo2Action : public BossAction
{
public:
	 BossSlashCombo2Action();//�R���X�g���N�^
	~BossSlashCombo2Action();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	bool	isClose;//��x�ł��߂Â�����
	float	targetOffset;
	float	hitStopTime;
	int		hitStopDelay;
	int		hitStopType;
	float	slowFactor;
	VECTOR	rotateLerpValue;	//��]�␳��
	short	rotateFixFrame;		//��]�␳�p
	short	moveFrame;			//�ړ��p
	short	rotateFrame1;		//��]�p
	short	rotateFrame2;		//��]�p
	float	stopDistance;		//��~���鋗��
	float	speed;				//���x
	short	maxInterval;		//�ő�C���^�[�o��
	short	checkState;			//�ǂ̏�ԂŊm�F���邩
	float	actionDistance;		//�A�N�V�������\�ɂȂ鋗��
	short   normalDisireValue;
};

