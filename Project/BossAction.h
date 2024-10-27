//================================================
// @brief �{�X�A�N�V�����̊��N���X
//================================================
#pragma once

class Boss;
class ActionParameter;
class BossAttack;
class HitStop;
class BossAction abstract
{
public:
			 BossAction();	//�R���X�g���N�^
	virtual ~BossAction(){}	//�f�X�g���N�^
	virtual void Initialize	  ()					abstract;//������
	virtual void Update		  (Boss& _boss)			abstract;//�X�V
	virtual void CalcParameter(const Boss& _boss)	abstract;//�p�����[�^�[�̌v�Z
	const void Draw()const;

	/*getter/setter*/
	const int  GetDesireValue()	const;
	const bool GetIsSelect	 ()	const { return this->isSelect; }
		  void OnIsSelect	 () { this->isSelect = true; }
	const int  GetWeight	 (const int _sum)const;
	const int GetBaseWeight	 ()const;
	const bool GetIsPriority ()const { return this->isPriority; }
protected:
	/*���������֐�*/
	bool	FrameCount		(const int _maxFrame);																		//�t���[���J�E���g
	void	OffIsSelect		(const int _maxInterval);																	//�I���t���O�̏�����
	VECTOR	GetLerpRotation (Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue);	//�⊮������]���̎擾
	VECTOR  CalcVelocity	(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed);//�ړ��x�N�g���̌v�Z

	/*�����o�ϐ�*/
	BossAttack*				 attack;		//�{�X�U���N���X
	HitStop*				 hitStop;		//�q�b�g�X�g�b�v
	ActionParameter*		 parameter;		//�p�����[�^
	VECTOR					 moveTarget;	//�ړ��ڕW
	bool					 isSelect;		//�A�N�V�������I�����ꂽ��
	int						 frameCount;	//�t���[���J�E���g
	bool					 isInitialize;	//���������ꂽ��
	bool					 isPriority;	//�D��t���O
	bool					 isAllowAction;	//�A�N�V������������
};

